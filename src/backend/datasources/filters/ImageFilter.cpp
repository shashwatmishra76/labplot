/***************************************************************************
File                 : ImageFilter.cpp
Project              : LabPlot
Description          : Image I/O-filter
--------------------------------------------------------------------
Copyright            : (C) 2015 by Stefan Gerlach (stefan.gerlach@uni.kn)
***************************************************************************/

/***************************************************************************
*                                                                         *
*  This program is free software; you can redistribute it and/or modify   *
*  it under the terms of the GNU General Public License as published by   *
*  the Free Software Foundation; either version 2 of the License, or      *
*  (at your option) any later version.                                    *
*                                                                         *
*  This program is distributed in the hope that it will be useful,        *
*  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
*  GNU General Public License for more details.                           *
*                                                                         *
*   You should have received a copy of the GNU General Public License     *
*   along with this program; if not, write to the Free Software           *
*   Foundation, Inc., 51 Franklin Street, Fifth Floor,                    *
*   Boston, MA  02110-1301  USA                                           *
*                                                                         *
***************************************************************************/
#include "backend/datasources/filters/ImageFilter.h"
#include "backend/datasources/filters/ImageFilterPrivate.h"
#include "backend/datasources/FileDataSource.h"
#include "backend/core/column/Column.h"

#include <math.h>

#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <KLocale>

 /*!
	\class ImageFilter
	\brief Manages the import/export of data from/to an image file.

	\ingroup datasources
 */
ImageFilter::ImageFilter():AbstractFileFilter(), d(new ImageFilterPrivate(this)){

}

ImageFilter::~ImageFilter(){
	delete d;
}

/*!
  reads the content of the file \c fileName to the data source \c dataSource.
*/
void ImageFilter::read(const QString & fileName, AbstractDataSource* dataSource, AbstractFileFilter::ImportMode importMode){
	d->read(fileName, dataSource, importMode);
}

/*!
writes the content of the data source \c dataSource to the file \c fileName.
*/
void ImageFilter::write(const QString & fileName, AbstractDataSource* dataSource){
 	d->write(fileName, dataSource);
// 	emit()
}

///////////////////////////////////////////////////////////////////////
/*!
  loads the predefined filter settings for \c filterName
*/
void ImageFilter::loadFilterSettings(const QString& filterName){
	Q_UNUSED(filterName);
}

/*!
  saves the current settings as a new filter with the name \c filterName
*/
void ImageFilter::saveFilterSettings(const QString& filterName) const{
	Q_UNUSED(filterName);
}

///////////////////////////////////////////////////////////////////////

void ImageFilter::setAutoModeEnabled(bool b){
	d->autoModeEnabled = b;
}

bool ImageFilter::isAutoModeEnabled() const{
	return d->autoModeEnabled;
}
//#####################################################################
//################### Private implementation ##########################
//#####################################################################

ImageFilterPrivate::ImageFilterPrivate(ImageFilter* owner) :
	q(owner){
}

/*!
    reads the content of the file \c fileName to the data source \c dataSource.
    Uses the settings defined in the data source.
*/
void ImageFilterPrivate::read(const QString & fileName, AbstractDataSource* dataSource, AbstractFileFilter::ImportMode mode){
	Q_UNUSED(dataSource)
	Q_UNUSED(mode)
#ifdef QT_DEBUG
	qDebug()<<"ImageFilterPrivate::read()";
#endif

	QImage image = QImage(fileName);
	if(image.isNull() || image.format() == QImage::Format_Invalid) {
		qDebug()<<"failed to read image"<<fileName<<"or invalid image format";
		return;
	}

	qDebug()<<"image format ="<<image.format();
	int cols = image.width();
	int rows = image.height();
	qDebug()<<"image w/h ="<<cols<<rows;

	QStringList vectorNameList;
	for (int k=0; k<cols; k++ )
		vectorNameList.append( "Column " + QString::number(k+1) );

	//make sure we have enough columns in the data source.
	Column * newColumn;
	int columnOffset=0; //indexes the "start column" in the spreadsheet. Starting from this column the data will be imported.
	dataSource->setUndoAware(false);
	
	if (mode==AbstractFileFilter::Append){
		columnOffset=dataSource->childCount<Column>();
		for ( int n=1; n<=cols; n++ ){
			newColumn = new Column(vectorNameList.at(n-1), AbstractColumn::Numeric);
			newColumn->setUndoAware(false);
			dataSource->addChild(newColumn);
		}
	}else if (mode==AbstractFileFilter::Prepend){
		Column* firstColumn = dataSource->child<Column>(0);
		for ( int n=1; n<=cols; n++ ){
			newColumn = new Column(vectorNameList.at(n-1), AbstractColumn::Numeric);
			newColumn->setUndoAware(false);
			dataSource->insertChildBefore(newColumn, firstColumn);
		}
	}else if (mode==AbstractFileFilter::Replace){
		//replace completely the previous content of the data source with the content to be imported.
		int columns = dataSource->childCount<Column>();

		if (columns > cols){
			//there're more columns in the data source then required
			//-> remove the superfluous columns
			for(int i=0;i<columns-cols;i++) {
				dataSource->removeChild(dataSource->child<Column>(0));
			}

			//rename the columns, that are already available
			for (int i=0; i<cols-1; i++){
				dataSource->child<Column>(i)->setUndoAware(false);
				dataSource->child<Column>(i)->setColumnMode( AbstractColumn::Numeric);
				dataSource->child<Column>(i)->setName(vectorNameList.at(i+1));
				dataSource->child<Column>(i)->setSuppressDataChangedSignal(true);
			}
		}else{
			//rename the columns, that are already available
			for (int i=0; i<columns; i++){
				dataSource->child<Column>(i)->setUndoAware(false);
				dataSource->child<Column>(i)->setColumnMode( AbstractColumn::Numeric);
				dataSource->child<Column>(i)->setName(vectorNameList.at(i));
				dataSource->child<Column>(i)->setSuppressDataChangedSignal(true);
			}

			//create additional columns if needed
			for(int i=columns; i < cols; i++) {
				newColumn = new Column(vectorNameList.at(i), AbstractColumn::Numeric);
				newColumn->setUndoAware(false);
				dataSource->addChild(newColumn);
				dataSource->child<Column>(i)->setSuppressDataChangedSignal(true);
			}
		}
	}

	// resize the spreadsheet
	Spreadsheet* spreadsheet = dynamic_cast<Spreadsheet*>(dataSource);
	if (mode==AbstractFileFilter::Replace) {
		spreadsheet->clear();
		spreadsheet->setRowCount(rows);
	}else{
		if (spreadsheet->rowCount() < rows)
			spreadsheet->setRowCount(rows);
	}

	// pointers to the actual data containers
	QVector<QVector<double>*> dataPointers;
	for ( int n=1; n<=cols; n++ ){
		QVector<double>* vector = static_cast<QVector<double>* >(dataSource->child<Column>(columnOffset+n-1)->data());
		vector->reserve(rows);
		vector->resize(rows);
		dataPointers.push_back(vector);
	}

	// read data
	for (int i=0; i<rows; i++){
		for ( int j=0; j<cols; j++ ){
			double value=qGray(image.pixel(i, j));
			dataPointers[j]->operator[](i) = value;
		}
	}

	QString comment = i18np("numerical data, %1 element", "numerical data, %1 elements", rows);
	for ( int n=1; n<=cols; n++ ){
		Column* column = spreadsheet->column(columnOffset+n-1);
		column->setComment(comment);
		column->setUndoAware(true);
		if (mode==AbstractFileFilter::Replace) {
			column->setSuppressDataChangedSignal(false);
			column->setChanged();
		}
	}

	spreadsheet->setUndoAware(true);
}

/*!
    writes the content of \c dataSource to the file \c fileName.
*/
void ImageFilterPrivate::write(const QString & fileName, AbstractDataSource* dataSource){
	Q_UNUSED(fileName);
	Q_UNUSED(dataSource);
	//TODO
}

//##############################################################################
//##################  Serialization/Deserialization  ###########################
//##############################################################################

/*!
  Saves as XML.
 */
void ImageFilter::save(QXmlStreamWriter* writer) const {
	writer->writeStartElement("imageFilter");
	writer->writeEndElement();
}

/*!
  Loads from XML.
*/
bool ImageFilter::load(XmlStreamReader* reader) {
	if(!reader->isStartElement() || reader->name() != "imageFilter"){
		reader->raiseError(i18n("no binary filter element found"));
		return false;
	}

	QString attributeWarning = i18n("Attribute '%1' missing or empty, default value is used");
	QXmlStreamAttributes attribs = reader->attributes();

	return true;
}