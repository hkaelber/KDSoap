#include "mainwindow.h"

#include <QPushButton>
#include <QLabel>
#include <QMovie>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QString>
#include <QDebug>
#include <QMessageBox>

#include "KDSoapPendingCallWatcher.h"

MainWindow::MainWindow( QWidget *parent ) : QWidget( parent )
{
   
  mBtnSync = new QPushButton(tr("Sync Call"), this);
  mBtnAsync = new QPushButton(tr("Async Call"), this);
  mLblResult = new QLabel(tr("Result: "), this);
  mLblAnim = new QLabel(this);
  
  mMovAnim = new QMovie(QString::fromAscii(":/animations/anim.gif"));
  
  QVBoxLayout *centralLayout = new QVBoxLayout(this);
  QHBoxLayout *btnsLayout = new QHBoxLayout();
  
  btnsLayout->addWidget( mBtnSync  );
  btnsLayout->addWidget( mBtnAsync );
  
  centralLayout->addLayout(btnsLayout);
  centralLayout->addWidget( mLblResult );
  centralLayout->addWidget( mLblAnim );
  
  mLblAnim->setMovie(mMovAnim);
  mMovAnim->start();
  
  connect(mBtnSync, SIGNAL(clicked()), this, SLOT(syncCall()));
  connect(mBtnAsync, SIGNAL(clicked()), this, SLOT(asyncCall()));
  
  mHolidayDates = new USHolidayDates();
  connect(mHolidayDates, SIGNAL(getValentinesDayDone(const TNS__GetValentinesDayResponse&)), 
          this,          SLOT(  getValentinesDay    (const TNS__GetValentinesDayResponse&)));
  
  connect(mHolidayDates, SIGNAL(getValentinesDayError(const KDSoapMessage&)),
          this,          SLOT  (getValentinesDayError(const KDSoapMessage&)));
  
  mYear = 1960;
  mParameters.setYear(mYear++);                 
}

void MainWindow::getValentinesDay(const TNS__GetValentinesDayResponse& response)
{
    mLblResult->setText( response.getValentinesDayResult().toString());
    
}

void MainWindow::getValentinesDayError(const KDSoapMessage& error)
{
    QMessageBox::warning(this, tr("Error retrieving valentines day"), error.faultAsString());
}
  
void MainWindow::syncCall()
{
    TNS__GetValentinesDayResponse response = mHolidayDates->getValentinesDay(mParameters);
    mLblResult->setText( response.getValentinesDayResult().toString());
    mParameters.setYear( mYear++ );
}

void MainWindow::asyncCall()
{
  mHolidayDates->asyncGetValentinesDay(mParameters);
  mParameters.setYear( mYear++ );
}