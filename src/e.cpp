#include <QMessageBox>
#include <stdlib.h>

#include "common.h"
#include "e.h"

void err(QString msg)
{
	cout << "Error: " << msg << endl;
	QMessageBox box(QMessageBox::Critical, "Xombie", msg);
	box.exec();
	exit(-1);
}

void warn(QString msg)
{
	cout << "Warning: " << msg << endl;
}

