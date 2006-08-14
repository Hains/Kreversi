#ifndef KREVERSI_MAIN_WINDOW_H
#define KREVERSI_MAIN_WINDOW_H

#include <kmainwindow.h>

class KReversiScene;
class KReversiGame;

class KReversiMainWindow : public KMainWindow
{
    Q_OBJECT
public:
    KReversiMainWindow(QWidget* parent=0);
public slots:
    void newGame();
    void slotBackgroundChanged(const QString& text);
private:
    void setupActions();

    KReversiScene *m_scene;
    KReversiGame *m_game;
};
#endif
