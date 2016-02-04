#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <iostream>
#include <cstdio>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  connect(ui->m_execute,SIGNAL(clicked()),this,SLOT(executeCommand()));
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::executeCommand()
{
  QString cmd=ui->m_lineEdit->text();
  QString out;
  FILE *in;
  char buff[512];

  if(!(in = popen(cmd.toLatin1()  , "r")))
  {
    out="popen failed";
  }

  while(fgets(buff, sizeof(buff), in)!=NULL)
  {
    out+=buff;
  }
  pclose(in);
  ui->m_output->setText(out);
}
