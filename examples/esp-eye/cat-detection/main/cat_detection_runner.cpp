#include "who_camera.h"
#include "who_cat_face_detection.hpp"
#include "cat_detection_runner.h"

static QueueHandle_t xQueueAIFrame = NULL;

void cat_detection_runner()
{
    xQueueAIFrame = xQueueCreate(2, sizeof(camera_fb_t *));

    register_camera(PIXFORMAT_RGB565, FRAMESIZE_240X240, 2, xQueueAIFrame);
    register_cat_face_detection(xQueueAIFrame, NULL, NULL, NULL, true);
}
