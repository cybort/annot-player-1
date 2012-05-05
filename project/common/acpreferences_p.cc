// acpreferences_p.cc
// 5/5/2012

#include "project/common/acpreferences_p.h"
#include "project/common/acui.h"
#include "project/common/acpaths.h"
#include "project/common/acsettings.h"
#include <QtGui>

// - Locations -

AcLocationPreferences::AcLocationPreferences(AcSettings *settings, QWidget *parent)
  : Base(settings, parent)
{
  locationManager_ = AcLocationManager::globalInstance();
  setWindowTitle(tr("Locations"));
  createLayout();
}

void
AcLocationPreferences::createLayout()
{
  AcUi *ui = AcUi::globalInstance();

  QStringList defvals(locationManager_->defaultDownloadsLocation());
  downloadsLocationEdit_ = ui->makeComboBox(
        AcUi::EditHint, "", tr("Downloads Location for Annot Player and Annot Downloader"), defvals.first(), defvals);
  downloadsLocationEdit_->setStatusTip(tr("Downloads Location"));
  connect(downloadsLocationEdit_, SIGNAL(editTextChanged(QString)), SLOT(verifyLocation(QString)));
  QToolButton *downloadsLocationButton = ui->makeToolButton(0, tr("Downloads"), tr("Downloads Location"), locationManager_, SLOT(openDownloadsLocation()));

  // Layouts
  QGridLayout *grid = new QGridLayout; {
    // (row, col, rowspan, colspan, alignment)
    int r, c;
    grid->addWidget(downloadsLocationButton, r=0, c=0, 1, 1);
    grid->addWidget(downloadsLocationEdit_, r, ++c,   1, 3);

    //grid->addWidget(passwordLabel, ++r, c=0);
    //grid->addWidget(passwordEdit_, r, ++c);
//
    //grid->addWidget(cancelButton, ++r, c=0, Qt::AlignHCenter);
    //grid->addWidget(loginButton, r, ++c, Qt::AlignHCenter);

    grid->setContentsMargins(9, 9, 9, 9);
    setContentsMargins(0, 0, 0, 0);
  } setLayout(grid);

  // Connections
  //connect(userNameEdit_->lineEdit(), SIGNAL(returnPressed()), SLOT(login()));
  //connect(passwordEdit_, SIGNAL(returnPressed()), SLOT(login()));
}

void
AcLocationPreferences::load()
{
  QString downloadsLocation = AcLocationManager::globalInstance()->downloadsLocation();
  downloadsLocationEdit_->setEditText(downloadsLocation);
  if (downloadsLocationEdit_->count() && downloadsLocation != downloadsLocationEdit_->itemText(0))
    downloadsLocationEdit_->addItem(downloadsLocation);
}

void
AcLocationPreferences::save()
{
  QString location = downloadsLocationEdit_->currentText().trimmed();
  if (location.isEmpty())
    location = locationManager_->defaultDownloadsLocation();
  settings()->setDownloadsLocation(location);
}

void
AcLocationPreferences::verifyLocation(const QString &path)
{
  if (QFile::exists(path))
    emit message(tr("exists") + ": " + path);
  else
    emit warning(tr("not exist") + ": " + path);
}

// EOF