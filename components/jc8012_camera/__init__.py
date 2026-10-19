"""JC8012 camera: the camera in the top bar while Home Assistant takes pictures of the screen's own camera.

The camera itself is esp_video_camera beside this folder (plugin.yaml); this component listens to it, as ESPHome's API
does, and says on the screen when pictures leave it. It also keeps what the camera said in the log while it started and
shows it in a text sensor (`status`): the camera starts before Home Assistant connects, so its log is gone by then.
And it reads the room's light level (`illuminance`) and motion (`motion`) from the camera's frames on the screen.
register_plugin() takes the id, version and the screen's texts from the manifest and translations/ beside this folder.
"""
import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor, logger, sensor, smart_display, text_sensor
from esphome.const import (
    CONF_ID,
    DEVICE_CLASS_ILLUMINANCE,
    DEVICE_CLASS_MOTION,
    STATE_CLASS_MEASUREMENT,
    UNIT_LUX,
)

DEPENDENCIES = ["smart_display", "camera", "logger"]
AUTO_LOAD = ["sensor", "binary_sensor"]

CONF_CAMERA = "camera"
CONF_STATUS = "status"
CONF_ACTIVITY = "activity"
CONF_ILLUMINANCE = "illuminance"
CONF_MOTION = "motion"

jc8012_camera_ns = cg.esphome_ns.namespace("jc8012_camera")
# ESPHome's camera base (esphome/components/camera/camera.h), which declares no class of its own in Python.
Camera = cg.esphome_ns.namespace("camera").class_("Camera", cg.EntityBase, cg.Component)
JC8012Camera = jc8012_camera_ns.class_("JC8012Camera", cg.Component)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(JC8012Camera),
        cv.Required(CONF_CAMERA): cv.use_id(Camera),
        cv.Optional(CONF_STATUS): cv.use_id(text_sensor.TextSensor),
        cv.Optional(CONF_ACTIVITY): cv.use_id(text_sensor.TextSensor),
        cv.Optional(CONF_ILLUMINANCE): sensor.sensor_schema(
            unit_of_measurement=UNIT_LUX,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_ILLUMINANCE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_MOTION): binary_sensor.binary_sensor_schema(device_class=DEVICE_CLASS_MOTION),
    }
).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await smart_display.register_plugin(var, __file__)
    cg.add(var.set_camera(await cg.get_variable(config[CONF_CAMERA])))
    if CONF_STATUS in config:
        cg.add(var.set_status(await cg.get_variable(config[CONF_STATUS])))
        # The camera starts in setup(), before this component's own; listening from here hears it.
        logger.request_log_listener()
        cg.add(var.listen_to_log())
    if CONF_ACTIVITY in config:
        cg.add(var.set_activity(await cg.get_variable(config[CONF_ACTIVITY])))
    if CONF_ILLUMINANCE in config:
        cg.add(var.set_illuminance(await sensor.new_sensor(config[CONF_ILLUMINANCE])))
    if CONF_MOTION in config:
        cg.add(var.set_motion(await binary_sensor.new_binary_sensor(config[CONF_MOTION])))
