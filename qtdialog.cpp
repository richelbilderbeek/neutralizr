#include "qtdialog.h"
#include "ui_qtdialog.h"

#include "metacommunity.h"
#include <sstream>

qtdialog::qtdialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::qtdialog)
{
  ui->setupUi(this);

  connect(ui->box_meta_j_m, SIGNAL(valueChanged(int)), this, SLOT(OnMetaChanged()));
  connect(ui->box_meta_rng_seed, SIGNAL(valueChanged(int)), this, SLOT(OnMetaChanged()));
  connect(ui->box_meta_theta, SIGNAL(valueChanged(double)), this, SLOT(OnMetaChanged()));
}

qtdialog::~qtdialog()
{
  delete ui;
}

metacommunity_parameters qtdialog::get_meta() const
{
  return metacommunity_parameters(
    ui->box_meta_j_m->value(),
    ui->box_meta_rng_seed->value(),
    ui->box_meta_theta->value()
  );
}

void qtdialog::OnMetaChanged()
{
  const metacommunity_parameters m = get_meta();
  const metacommunity c(m);
  std::stringstream s;
  s << "Parameters:\n" << m << '\n'
    << "Community: " << c << '\n';
  ui->log->setPlainText(s.str().c_str());
}
