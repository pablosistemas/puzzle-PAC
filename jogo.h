#ifndef JOGO_H
#define JOGO_H

#include <QObject>
#include <QStringList>
#include <QTime>

class Jogo : public QObject
{
    Q_OBJECT
public:
    explicit Jogo(QObject *parent = 0);

signals:

public slots:
    bool puzzleMovement(int);
    void createNewGame();

public:
    QStringList randomInitTexts() const;
    QStringList getMatrizPosicoes() const;
    void swapPecas(int, int);
    QTime getTime() const;
    int getNumMovements() const;
    void setTime(QTime);
    bool testVictory() const;

private:
    int numMovimentos;
    QTime tempoJogo;
    QStringList matrizDePosicoes;
    QWidget *victory;
};

#endif // JOGO_H
