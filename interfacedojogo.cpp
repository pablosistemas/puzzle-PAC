#include "interfacedojogo.h"
#include "ui_interfacedojogo.h"

#include <QTextStream>
#include <QDir>
#include <QStringListModel>
#include <QListView>
#include <QDebug>
#include <QListWidget>
#include <QtAlgorithms>
#include <QHBoxLayout>
#include <QLabel>
#include <QInputDialog>

// sorting functions prototypes
bool sortingString2Time(QString, QString);
bool sortingString2Int(QString, QString);

InterfaceDoJogo::InterfaceDoJogo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InterfaceDoJogo)
{
    ui->setupUi(this);

    // initializes the pieces
    jogo = new Jogo(this);

    // initializes the piece's texts randomly
    QStringList initText = jogo->getMatrizPosicoes();

    // maps the push buttons to int values corresponding the piece position number
    signalMapper = new QSignalMapper(this);

    // initializes Pecas
    // schedules objects to be deleted after control return for event loop

    Pecas.append(QSharedPointer<QPushButton>(ui->peca1, &QObject::deleteLater));
    Pecas.append(QSharedPointer<QPushButton>(ui->peca2, &QObject::deleteLater));
    Pecas.append(QSharedPointer<QPushButton>(ui->peca3, &QObject::deleteLater));
    Pecas.append(QSharedPointer<QPushButton>(ui->peca4, &QObject::deleteLater));
    Pecas.append(QSharedPointer<QPushButton>(ui->peca5, &QObject::deleteLater));
    Pecas.append(QSharedPointer<QPushButton>(ui->peca6, &QObject::deleteLater));
    Pecas.append(QSharedPointer<QPushButton>(ui->peca7, &QObject::deleteLater));
    Pecas.append(QSharedPointer<QPushButton>(ui->peca8, &QObject::deleteLater));
    Pecas.append(QSharedPointer<QPushButton>(ui->peca9, &QObject::deleteLater));
    Pecas.append(QSharedPointer<QPushButton>(ui->peca10, &QObject::deleteLater));
    Pecas.append(QSharedPointer<QPushButton>(ui->peca11, &QObject::deleteLater));
    Pecas.append(QSharedPointer<QPushButton>(ui->peca12, &QObject::deleteLater));
    Pecas.append(QSharedPointer<QPushButton>(ui->peca13, &QObject::deleteLater));
    Pecas.append(QSharedPointer<QPushButton>(ui->peca14, &QObject::deleteLater));
    Pecas.append(QSharedPointer<QPushButton>(ui->peca15, &QObject::deleteLater));
    Pecas.append(QSharedPointer<QPushButton>(ui->peca16, &QObject::deleteLater));

    updatePecas(initText);

    for(int i = 0; i < 16; ++i){
        // schedules objects to be deleted after control return for event loop
        connect(Pecas[i].data(), SIGNAL(clicked()), signalMapper, SLOT(map()));
        signalMapper->setMapping(Pecas[i].data(), i);
    }
    // connects event related to pecas
    connect(signalMapper, SIGNAL(mapped(int)),
            this, SLOT(puzzleMovement(int)));

    // connects event related to Novo Jogo
    connect(ui->newGame, SIGNAL(clicked()),
            this, SLOT(createNewGame()));

    // connects event of hall of fame
    connect(ui->hallOfFame, SIGNAL(clicked()),
            this, SLOT(showHallOfFame()));


    // defines the timer for the game
    ui->gameTimer->setDigitCount(8);
    ui->gameTimer->setSegmentStyle(QLCDNumber::Filled);

    timer = new QTimer(this);
    timer->start(1000);

    // connects the chronometer event
    connect(timer, SIGNAL(timeout()), this, SLOT(updateTimer()));

    // on the startup we enable the hallOfFame button whether tempos.txt or movimentos.txt file exists
    // We presume that whether tempo.txt exists, moviments.txt also exists in the folder
    QFile t;
    QDir dir(QDir::currentPath());

    t.setFileName(dir.absoluteFilePath("tempos.txt"));

    if(t.exists())
        ui->hallOfFame->setEnabled(true);

}

/*void InterfaceDoJogo::puzzleMovement(int idx){

    bool isVictory = jogo->puzzleMovement(idx);
    updatePecas(jogo->getMatrizPosicoes());

    // Shows message of victory
    QMessageBox victoryDialog;

    // when victory, shows dialog message and record in the file
    // the time and num of movements spent
    if(isVictory)
    {
        QTime tempoVitoria = jogo->getTime();
        int numMovimentos = jogo->getNumMovements();
        QString msg("Você ganhou!\nNúmero de jogadas: "+ QString::number(numMovimentos)+
                    "\nTempo de jogo: "+tempoVitoria.toString());
        victoryDialog.setText(msg);
        victoryDialog.exec();

        // records in File
        recordFile(tempoVitoria,numMovimentos);
    }

}*/

void InterfaceDoJogo::puzzleMovement(int idx){

    bool isVictory = jogo->puzzleMovement(idx);
    updatePecas(jogo->getMatrizPosicoes());

    // Shows message of victory
    QInputDialog victoryDialog;

    // when victory, shows dialog message and record in the file
    // the time and num of movements spent
    if(isVictory)
    {
        QTime tempoVitoria = jogo->getTime();
        int numMovimentos = jogo->getNumMovements();
        QString msg("Você ganhou!\nNúmero de jogadas: "+ QString::number(numMovimentos)+
                    "\nTempo de jogo: "+tempoVitoria.toString());

        QDialog *parent         = new QDialog();
        QGroupBox *boxUser      = new QGroupBox(parent);
        QGroupBox *boxCongrat   = new QGroupBox(parent);
        QHBoxLayout *nomeUser   = new QHBoxLayout(boxUser);
        QHBoxLayout *congrat    = new QHBoxLayout(boxCongrat);
        QVBoxLayout *vlayout    = new QVBoxLayout(parent);

        QLabel *inputNome = new QLabel(QString("Digite seu nome:"),boxUser);
        QLineEdit *linhaNome = new QLineEdit(boxUser);

        nomeUser->addWidget(inputNome);
        nomeUser->addWidget(linhaNome);

        QLabel *parabens = new QLabel(msg,boxCongrat);
        congrat->addWidget(parabens);

        vlayout->addWidget(boxCongrat);
        vlayout->addWidget(boxUser);

        parent->exec();

        QString nome = ("anonymous");

        // if user specify his/her name, saves, else, we use a  name
        if(!linhaNome->text().isEmpty()){
            nome = linhaNome->text();
        }

        delete parent;

        // records in File
        recordInFile(tempoVitoria,numMovimentos,nome);
    }

}

void InterfaceDoJogo::createNewGame() {
    jogo->createNewGame();
    updatePecas(jogo->getMatrizPosicoes());
    // TODO: updates chronometer

}

void InterfaceDoJogo::updateTimer() {
    // TODO: updates chronometer
    QTime t = jogo->getTime().addSecs(1);
    QString text = t.toString("hh:mm:ss");
    jogo->setTime(t);

    ui->gameTimer->display(text);
}

void InterfaceDoJogo::setTextToPiece(QPushButton *ptr, QString t){
    if(t == QString::number(16))
        ptr->setVisible(false);
    else
        ptr->setVisible(true);

    ptr->setText(t);
}

void InterfaceDoJogo::updatePecas(QStringList ql){
    setTextToPiece(ui->peca1,ql[0]);
    setTextToPiece(ui->peca2,ql[1]);
    setTextToPiece(ui->peca3,ql[2]);
    setTextToPiece(ui->peca4,ql[3]);
    setTextToPiece(ui->peca5,ql[4]);
    setTextToPiece(ui->peca6,ql[5]);
    setTextToPiece(ui->peca7,ql[6]);
    setTextToPiece(ui->peca8,ql[7]);
    setTextToPiece(ui->peca9,ql[8]);
    setTextToPiece(ui->peca10,ql[9]);
    setTextToPiece(ui->peca11,ql[10]);
    setTextToPiece(ui->peca12,ql[11]);
    setTextToPiece(ui->peca13,ql[12]);
    setTextToPiece(ui->peca14,ql[13]);
    setTextToPiece(ui->peca15,ql[14]);
    setTextToPiece(ui->peca16,ql[15]);
}

InterfaceDoJogo::~InterfaceDoJogo()
{
    delete ui;
    delete jogo;
    delete signalMapper;
}

void InterfaceDoJogo::recordInFile(QTime tempo, int movimentos, QString nome) {
    // enables hall of fame pushbutton if there is at least one record in file
    if(!ui->hallOfFame->isEnabled()){
        ui->hallOfFame->setEnabled(true);
    }

    QFile output;
    QDir dir(QDir::currentPath());

    output.setFileName(dir.absoluteFilePath("movimentos.txt"));
    output.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream out1(&output);

    // writes the number of movements and the name of the user separated by "/"
    // we use split to separate these two fields again when recording the file
    out1 << movimentos << "/" << nome << endl;
    output.close();

    output.setFileName("tempos.txt");
    output.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream out2(&output);
    out2 << tempo.toString() << "/" << nome << endl;
    output.close();
}

void InterfaceDoJogo::showHallOfFame(){
    QStringList rankingOfTimes;
    QStringList rankingOfMovements;

    QFile input;
    QDir dir(QDir::currentPath());

    input.setFileName(dir.absoluteFilePath("movimentos.txt"));
    input.open(QIODevice::ReadOnly);
    QTextStream stream(&input);
    QString line;

    line = stream.readLine();
    while(!line.isNull()){
        rankingOfMovements.append(line);
        line = stream.readLine();
    }
    input.close();

    input.setFileName("tempos.txt");
    input.open(QIODevice::ReadOnly);
    stream.setDevice(&input);

    line = stream.readLine();
    while(!line.isNull()){
        rankingOfTimes.append(line);
        line = stream.readLine();
    }
    input.close();

    // sorts the list to exhibit on the screen
    qSort(rankingOfTimes.begin(), rankingOfTimes.end(), sortingString2Time);
    qSort(rankingOfMovements.begin(), rankingOfMovements.end(), sortingString2Int);

    // Collapses two listViews inside one widget
    QWidget *windowRankings = new QWidget();

    QVBoxLayout *vlayout = new QVBoxLayout;

    QGroupBox * group1 = new QGroupBox;
    QGroupBox * group2 = new QGroupBox;

    QHBoxLayout *hlayout1 = new QHBoxLayout(group1);
    QHBoxLayout *hlayout2 = new QHBoxLayout(group2);

    QListWidget *leftList = new QListWidget;
    QListWidget *rightList = new QListWidget;

    leftList->addItems(rankingOfMovements);
    rightList->addItems(rankingOfTimes);

    hlayout2->addWidget(leftList);
    hlayout2->addWidget(rightList);

    QLabel *titulo1 = new QLabel("Movimentos/Jogador");
    QLabel *titulo2 = new QLabel("Tempo/Jogador");

    hlayout1->addWidget(titulo1);
    hlayout1->addWidget(titulo2);

    vlayout->addWidget(group1);
    vlayout->addWidget(group2);

    windowRankings->setLayout(vlayout);
    windowRankings->show();

    //showHallOfMovements(rankingOfMovements);
    //showHallOfTimes(rankingOfTimes);

}

// For now these methods are unused once a main wigdet creation method does all the job!
void InterfaceDoJogo::showHallOfTimes(QStringList times){
    // TODO: optimize and sort and delete
    QListView *listOfMoves = new QListView();
    QStringListModel *tempos = new QStringListModel(times);
    listOfMoves->setModel(tempos);
    listOfMoves->show();
}

void InterfaceDoJogo::showHallOfMovements(QStringList moves){
    // TODO: optimize and sort and delete
    QListView *listOfMoves = new QListView();
    QStringListModel *tempos = new QStringListModel(moves);
    listOfMoves->setModel(tempos);
    listOfMoves->show();
}

// the comparison function used by qsort have to aplit the strings
// once they are in the format desiredValue/playerName
// returns true if the first argument is less than the second one

// used to sort the time spent in game
bool sortingString2Time(QString t1, QString t2){
    QStringList splittedT1 = t1.split("/");
    QStringList splittedT2 = t2.split("/");

    QTime qt1 = QTime::fromString(splittedT1[0]);
    QTime qt2 = QTime::fromString(splittedT2[0]);

    return qt1.hour()*3600+qt1.minute()*60+qt1.second() < qt2.hour()*3600+qt2.minute()*60+qt2.second();
}

// used to sort the number of movements spent in the game
bool sortingString2Int(QString t1, QString t2){
    QStringList splittedT1 = t1.split("/");
    QStringList splittedT2 = t2.split("/");
    int it1 = splittedT1[0].toInt();
    int it2 = splittedT1[0].toInt();

    return it1 < it2;
}
