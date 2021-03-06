#ifndef IMPORT_H
#define IMPORT_H

#include <QtGui>
#include "ui_dialogImport.h"

#include <QMainWindow>

/*! Import é a classe que implementa a janela de diálogo para importar imagens RAW. */
class Import : public QDialog, public Ui::DialogImport
{
		Q_OBJECT

	public:
		explicit Import( QMainWindow * _parent, QString _file );

		~Import();

		QString fullFileName;

	private:
		QMainWindow *parent;
};

#endif
