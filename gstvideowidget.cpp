#include "gstvideowidget.h"
#include <gst/video/videooverlay.h>

GstVideoWidget::GstVideoWidget(QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_NativeWindow);

    gst_init(nullptr, nullptr);

    pipeline = gst_parse_launch(
        "rtspsrc location=rtsp://192.168.144.108 latency=0 protocols=udp ! "
        "rtph264depay ! h264parse ! d3d11h264dec ! "
        "d3d11videosink name=videosink sync=false",
        nullptr
        );

    videoSink = gst_bin_get_by_name(GST_BIN(pipeline), "videosink");
}

GstVideoWidget::~GstVideoWidget()
{
    if (pipeline) {
        gst_element_set_state(pipeline, GST_STATE_NULL);
        gst_object_unref(pipeline);
    }
}

void GstVideoWidget::showEvent(QShowEvent *)
{
    if (!videoSink)
        return;

    gst_video_overlay_set_window_handle(
        GST_VIDEO_OVERLAY(videoSink),
        (guintptr)winId()
        );

    gst_element_set_state(pipeline, GST_STATE_PLAYING);
}
