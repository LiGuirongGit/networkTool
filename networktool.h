#ifndef networkTool_H
#define networkTool_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class networkTool; }
QT_END_NAMESPACE

class networkTool : public QWidget
{
    Q_OBJECT

public:
    networkTool(QWidget *parent = nullptr);
    ~networkTool();

private slots:
    void slotConnectDone();
    void slotDisconnected();

private:
    Ui::networkTool *ui;
};
#endif // networkTool_H
