#ifndef QTDIALOG_H
#define QTDIALOG_H

#include <QDialog>
#include "metacommunity_parameters.h"

namespace Ui {
  class qtdialog;
}

class qtdialog : public QDialog
{
  Q_OBJECT

public:
  explicit qtdialog(QWidget *parent = 0);
  ~qtdialog();


  metacommunity_parameters get_meta() const;
private:
  Ui::qtdialog *ui;

public slots:
  void OnMetaChanged();
};

#endif // QTDIALOG_H
