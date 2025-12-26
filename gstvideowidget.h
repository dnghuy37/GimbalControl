#pragma once

#include <QWidget>
#include <gst/gst.h>

class GstVideoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GstVideoWidget(QWidget *parent = nullptr);
    ~GstVideoWidget();

protected:
    void showEvent(QShowEvent *event) override;

private:
    GstElement *pipeline = nullptr;
    GstElement *videoSink = nullptr;
};
