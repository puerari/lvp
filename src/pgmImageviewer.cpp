#include "pgmImageviewer.h"

PgmImageViewer::PgmImageViewer(QMainWindow * _parent) : BasePnmImageViewer( _parent ), pm(nullptr) {
	//setAttribute(Qt::WA_DeleteOnClose);
	//setBackgroundRole(QPalette::Shadow);
}

bool PgmImageViewer::loadFile(const QString &fileName) {
	if (!fileName.isEmpty()) {
		pm = new TCMatriz2D<int>(fileName.toStdString());
		if( pm ){
			getFileNames(fileName);
			if ( loadImage(pm)){
				return true;
			}
		}
	}
	QMessageBox::information(parent, tr("LVP"), tr("Error! - Cannot load %1.").arg(fileName));
	return false;
}

void PgmImageViewer::reloadFile() {
	if (pm) {
		delete pm;
		pm = nullptr;
	}
	pm = new TCMatriz2D<int>( fullFileName.toStdString() );
	if ( ! reloadImage(pm) ){
		QMessageBox::information(parent, tr("LVP"), tr("Error! - Cannot load image."));
	}
}

bool PgmImageViewer::save() {
	if (isNew) {
		return saveAs();
	} else {
		if( ! pm )
			pm = new TCMatriz2D<int>(fullFileName.toStdString());
		if (pm)
			return pm->Write(curFile.toStdString());
		else
			return false;
	}
}

bool PgmImageViewer::saveAs() {
	QString fileName;
	if (isNew){
		fileName = filePath + curFile;
	} else {
		fileName = fullFileName;
	}
	fileName = QFileDialog::getSaveFileName(parent, tr("Save As"), fileName, tr("pgm Images (*.pgm)"));
	if (fileName.isEmpty()){
		return false;
	}
	if ( ! pm ) {
		pm = new TCMatriz2D<int>(fullFileName.toStdString());
	}
	if ( ! pm ) {
		QMessageBox::information(parent, tr("LVP"), tr("Error! - Can't create image."));
		return false;
	}
	//
	QMessageBox msgBox(this);
	msgBox.setWindowTitle(tr("Save As"));
	msgBox.setText(tr("Saving Format:"));
	QPushButton *cancelButton = msgBox.addButton(QMessageBox::Cancel);
	msgBox.addButton(tr("&Binary"), QMessageBox::ActionRole);
	QPushButton *alignedAsciiButton = msgBox.addButton(tr("A&ligned ASCII"), QMessageBox::ActionRole);
	QPushButton *asciiButton = msgBox.addButton(tr("&ASCII"), QMessageBox::ActionRole);
	msgBox.setDefaultButton(asciiButton);
	msgBox.exec();
	if ( msgBox.clickedButton() == cancelButton )
		return false;
	if (msgBox.clickedButton() == asciiButton) { //ascii
		pm->SetFormato(P2_X_Y_GRAY_ASCII);
	} else if (msgBox.clickedButton() == alignedAsciiButton) { //ascii alinhado
		pm->SetFormato(P2_X_Y_GRAY_ASCII);
		pm->salvarAlinhado = true;
	} else { //binario.
		pm->SetFormato(P5_X_Y_GRAY_BINARY);
	}
	string tmp = pm->Path();
	pm->Path("");
	pm->salvarAlinhado = true;
	if( pm->Write( fileName.toStdString() ) ) {
		if ( isNew && (fileName != fullFileName) ) {
			QFile::remove(fullFileName);
		}
		isNew = false;
		loadFile(fileName);
		return true;
	}else{
		pm->Path(tmp);
		QMessageBox::information(parent, tr("LVP"), tr("Error! - Can't save file \"%1\"").arg(fileName));
		return false;
	}
}
