#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

// add to namespace Ui
namespace Ui {
class MainWindow;
}

/**
 * @brief The MainWindow class
 *
 * This class handles the QMainWindow that is defined
 * in the mainwindow.ui
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief MainWindow constructor
     * @param parent the parent widget
     */
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    static float _height;
    static float _width;

private:
    Ui::MainWindow *ui; /**< references all ui elements */

private slots:

    /*
     * Define slots to connect the ui elements with
     */

    void setAnimationSpeed(int value);
    void setLocalRotation(bool value);
    void setGlobalRotation(bool value);
    void setCameraRest();
    void setRing(int value);
    void setU(int value);
    void setV(int value);
    void setLaserLength(int value);
    void setLaser(bool value);
    void setLaserAngle(int value);
    void setGitter(bool value);
    void setLightOn(bool value);
//    void setLaserCone(bool value);
//    void setOrbit(bool value);
//    void setPath(bool value);



protected:
    virtual void keyPressEvent(QKeyEvent* event);
};

#endif // MAINWINDOW_H
