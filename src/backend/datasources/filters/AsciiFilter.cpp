/***************************************************************************
File                 : AsciiFilter.cpp
Project              : LabPlot/AbstractColumn
Description          : ASCII I/O-filter
--------------------------------------------------------------------
Copyright            : (C) 2009 by Stefan Gerlach (stefan.gerlach*uni-konstanz.de)
Copyright            : (C) 2009-2013 Alexander Semke (alexander.semke*web.de)
					   (replace * with @ in the email addresses) 

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
#include "backend/datasources/filters/AsciiFilter.h"
#include "backend/datasources/filters/AsciiFilterPrivate.h"
#include "backend/datasources/FileDataSource.h"
#include "backend/core/column/Column.h"

#include <math.h>

#include <QFile>
#include <QTextStream>
#include <QDebug>

 /*!
	\class AsciiFilter
	\brief Manages the import/export of data organized as columns (vectors) from/to an ASCII-file.

	\ingroup datasources
 */

AsciiFilter::AsciiFilter():AbstractFileFilter(), d(new AsciiFilterPrivate(this)){

}

AsciiFilter::~AsciiFilter(){
	delete d;
}

/*!
  reads the content of the file \c fileName to the data source \c dataSource.
*/
void AsciiFilter::read(const QString & fileName, AbstractDataSource* dataSource, AbstractFileFilter::ImportMode importMode){
  d->read(fileName, dataSource, importMode);
}


/*!
writes the content of the data source \c dataSource to the file \c fileName. 
*/
void AsciiFilter::write(const QString & fileName, AbstractDataSource* dataSource){
 	d->write(fileName, dataSource);
// 	emit()
}

/*!
  loads the predefined filter settings for \c filterName
*/
void AsciiFilter::loadFilterSettings(const QString& filterName){
    Q_UNUSED(filterName);
}

/*!
  saves the current settings as a new filter with the name \c filterName
*/
void AsciiFilter::saveFilterSettings(const QString& filterName) const{
    Q_UNUSED(filterName);
}

/*!
  returns the list with the names of all saved
  (system wide or user defined) filter settings.
*/
QStringList AsciiFilter::predefinedFilters(){
	return QStringList();
}

/*!
  returns the list of all predefined separator characters.
*/
QStringList AsciiFilter::separatorCharacters(){
  return (QStringList()<<"auto"<<"TAB"<<"SPACE"<<","<<";"<<":"
				  <<",TAB"<<";TAB"<<":TAB"
				  <<",SPACE"<<";SPACE"<<":SPACE");
}


/*!
returns the list of all predefined comment characters.
*/
QStringList AsciiFilter::commentCharacters(){
  return (QStringList()<<"#"<<"!"<<"//"<<"+"<<"c"<<":"<<";");
}

/*!
    returns the number of columns in the file \c fileName.
*/
int AsciiFilter::columnNumber(const QString & fileName){
  QString line;
  QStringList lineStringList;

  QFile file(fileName);
  if ( !file.exists() )
	  return 0;

	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	  return 0;

  QTextStream in(&file);
  line = in.readLine();
  lineStringList = line.split( QRegExp("\\s+"));
  return lineStringList.size();
}


/*!
  returns the number of lines in the file \c fileName.
*/
long AsciiFilter::lineNumber(const QString & fileName){
  //TODO: compare the speed of this function with the speed of wc from GNU-coreutils.
  QFile file(fileName);
  if ( !file.exists() )
	  return 0;

	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	  return 0;

  QTextStream in(&file);
  long rows=0;
  while (!in.atEnd()){
	  in.readLine();
	  rows++;
  }

  return rows;
}

void AsciiFilter::setTransposed(const bool b){
  d->transposed=b;
}

bool AsciiFilter::isTransposed() const{
  return d->transposed;
}

void AsciiFilter::setCommentCharacter(const QString& s){
  d->commentCharacter=s;
}

QString AsciiFilter::commentCharacter() const{
	return d->commentCharacter;
}

void AsciiFilter::setSeparatingCharacter(const QString& s){
  d->separatingCharacter=s;
}

QString AsciiFilter::separatingCharacter() const{
  return d->separatingCharacter;
}

void AsciiFilter::setAutoModeEnabled(bool b){
  d->autoModeEnabled=b;
}

bool AsciiFilter::isAutoModeEnabled() const{
  return d->autoModeEnabled;
}

void AsciiFilter::setHeaderEnabled(bool b){
  d->headerEnabled=b;
}

bool AsciiFilter::isHeaderEnabled() const{
  return d->headerEnabled;
}

void AsciiFilter::setVectorNames(const QString s){
  d->vectorNames=s.simplified();
}

QString AsciiFilter::vectorNames() const{
	return d->vectorNames;
}

void AsciiFilter::setEmptyLinesEnabled(bool b){
  d->emptyLinesEnabled=b;
}

bool AsciiFilter::emptyLinesEnabled() const{
  return d->emptyLinesEnabled;
}

void AsciiFilter::setSimplifyWhitespacesEnabled(bool b){
  d->simplifyWhitespacesEnabled=b;
}

bool AsciiFilter::simplifyWhitespacesEnabled() const{
  return d->simplifyWhitespacesEnabled;
}

void AsciiFilter::setStartRow(const int r){
  d->startRow=r;
}

int AsciiFilter::startRow() const{
  return d->startRow;
}

void AsciiFilter::setEndRow(const int r){
  d->endRow=r;
}

int AsciiFilter::endRow() const{
  return d->endRow;
}

void AsciiFilter::setStartColumn(const int c){
  d->startColumn=c;
}

int AsciiFilter::startColumn() const{
  return d->startColumn;
}

void AsciiFilter::setEndColumn(const int c){
  d->endColumn=c;
}

int AsciiFilter::endColumn() const{
  return d->endColumn;
}

//#####################################################################
//################### Private implementation ##########################
//#####################################################################
AsciiFilterPrivate::AsciiFilterPrivate(AsciiFilter* owner) : q(owner),
  commentCharacter("#"),
  separatingCharacter("auto"),
  autoModeEnabled(true),
  headerEnabled(true),
  emptyLinesEnabled(true),
  simplifyWhitespacesEnabled(true),
  transposed(false),
  startRow(0),
  endRow(-1),
  startColumn(0),
  endColumn(-1)
  {
}

/*!
    reads the content of the file \c fileName to the data source \c dataSource.
    Uses the settings defined in the data source.
*/
//TODO : read strings (comments) or datetime too
//TODO the terms vector and column are used synonymously, straighten the notation
void AsciiFilterPrivate::read(const QString & fileName, AbstractDataSource* dataSource, AbstractFileFilter::ImportMode mode){
	int currentRow=0; //indexes the position in the vector(column)
	int columnOffset=0; //indexes the "start column" in the spreadsheet. Starting from this column the data will be imported.
	QString line;
	QStringList lineStringList;

	QFile file(fileName);
	if ( !file.exists() )
		return;

     if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

	QTextStream in(&file);


	
	//TODO implement 
	// if (transposed)
	//...
	
	//skip rows, if required
	for (int i=0; i<startRow; i++){
        //if the number of rows to skip is bigger then the actual number
		//of the rows in the file, then quit the function.
		if( in.atEnd() ){
		  return;
		}
		
		in.readLine();
	}

	//parse the first row:
    //use the first row to determine the number of columns,
	//create the columns and use (optionaly) the first row to name them
    if( in.atEnd() ){
	  return;
	}
	
	line = in.readLine();
	if( simplifyWhitespacesEnabled)
		line = line.simplified();

	if( separatingCharacter == "auto" ){
	  lineStringList = line.split( QRegExp("\\s+"), (QString::SplitBehavior)emptyLinesEnabled );
    }else{
	  //TODO: doesn't work...
	  separatingCharacter.replace(QString("TAB"), QString("\t"), Qt::CaseInsensitive);
	  separatingCharacter.replace(QString("SPACE"), QString(" "), Qt::CaseInsensitive);
// 	  qDebug()<<"separator"<<separatingCharacter;
	  lineStringList = line.split( separatingCharacter, (QString::SplitBehavior)emptyLinesEnabled );
    }

	if (endColumn == -1)
		endColumn = lineStringList.size()-1; //use the last available column index

	QStringList vectorNameList;
	if ( headerEnabled ){
		vectorNameList = lineStringList;
	}else{
		//create vector names out of the vectorNames-string, if not empty
		if (vectorNames != ""){
			vectorNameList = vectorNames.split( QRegExp("\\s+") );
		}

		//if there were no (or not enough) strings provided, add the default descriptions for the columns/vectors
		if ( vectorNameList.size() < endColumn-startColumn+1 ) {
			int size=vectorNameList.size();
			for (int k=0; k<endColumn-startColumn+1-size; k++ )
				vectorNameList.append( "Column " + QString::number(size+k+1) );
		}
	}


	//make sure we have enough columns in the data source.
	//we need in total (endColumn-startColumn+1) columns.
	//Create new columns, if needed.
	Column * newColumn;
	dataSource->setUndoAware(false);
	if (mode==AbstractFileFilter::Append){
		columnOffset=dataSource->childCount<Column>();
		for ( int n=startColumn; n<=endColumn; n++ ){
			newColumn = new Column(vectorNameList.at(n), AbstractColumn::Numeric);
			newColumn->setUndoAware(false);
			dataSource->addChild(newColumn);
		}			
	}else if (mode==AbstractFileFilter::Prepend){
		Column* firstColumn = dataSource->child<Column>(0);
		for ( int n=startColumn; n<=endColumn; n++ ){
			newColumn = new Column(vectorNameList.at(n), AbstractColumn::Numeric);
			newColumn->setUndoAware(false);
			dataSource->insertChildBefore(newColumn, firstColumn);
		}
	}else if (mode==AbstractFileFilter::Replace){
		//replace completely the previous content of the data source with the content to be imported.
		int columns = dataSource->childCount<Column>();

		if (columns>(endColumn-startColumn+1)){
			//there're more columns in the data source then required
			//-> remove the superfluous columns
			for(int i=0;i<columns-(endColumn-startColumn+1);i++) {
				dataSource->removeChild(dataSource->child<Column>(0));
			}
			
			//rename the columns, that are already available
			for (int i=0; i<endColumn-startColumn; i++){
				dataSource->child<Column>(i)->setUndoAware(false);
				dataSource->child<Column>(i)->setColumnMode( AbstractColumn::Numeric);
				dataSource->child<Column>(i)->setName(vectorNameList.at(startColumn+i));
			}
		}else{
			//rename the columns, that are already available
			for (int i=0; i<columns; i++){
				dataSource->child<Column>(i)->setUndoAware(false);
				dataSource->child<Column>(i)->setColumnMode( AbstractColumn::Numeric);
				dataSource->child<Column>(i)->setName(vectorNameList.at(startColumn+i));
			}
			
			//create additional columns if needed
			for(int i=0; i<=(endColumn-startColumn-columns); i++) {
				newColumn = new Column(vectorNameList.at(columns+startColumn+i), AbstractColumn::Numeric);
				newColumn->setUndoAware(false);
				dataSource->addChild(newColumn);
			}
		}
	}


	int numLines=AsciiFilter::lineNumber(fileName);
	int actualEndRow;
	if (endRow == -1)
		actualEndRow = numLines;
	else if (endRow > numLines-1)
		actualEndRow = numLines-1;
	else
		actualEndRow = endRow;

	if (headerEnabled)
		numLines = actualEndRow-startRow;
	else
		numLines = actualEndRow-startRow+1;

	//resize the spreadsheet
	Spreadsheet* spreadsheet = dynamic_cast<Spreadsheet*>(dataSource);
	if (mode==AbstractFileFilter::Replace){
		spreadsheet->clear();
		spreadsheet->setRowCount(numLines);
	}else{
		if (spreadsheet->rowCount()<numLines)
			spreadsheet->setRowCount(numLines);
	}

	//pointers to the actual data containers
// 	QVector<QVector<double>*> dataPointers;
// 	for ( int n=startColumn; n<=endColumn; n++ ){
// 		QVector<double>* vector = static_cast<QVector<double>* >(dataSource->child<Column>(columnOffset+n-startColumn)->data());
// // 		vector->resize(numLines);
// 		dataPointers.push_back(vector);
// 	}
	
	//import the values in the first line, if they were not used as the header (as the names for the columns)
	if (!headerEnabled){
		for ( int n=startColumn; n<=endColumn; n++ ){
// 			dataPointers[n-startColumn]->insert(0, lineStringList.at(n).toDouble());
			Column* column = dataSource->child<Column>(columnOffset+n-startColumn);
			if (n<lineStringList.size())
				column->setValueAt(0, lineStringList.at(n).toDouble());
			else
				column->setValueAt(0, NAN);			
	  }
	  currentRow++;
	}

	//first line in the file is parsed. Read the remainder of the file.
	for (int i=0; i<numLines; i++){
		line = in.readLine();

		if( simplifyWhitespacesEnabled )
			line = line.simplified();

		if( line.startsWith(commentCharacter) == true ){
			currentRow++;
			continue;
		}

		if( separatingCharacter == "auto" )
			lineStringList = line.split( QRegExp("\\s+"), (QString::SplitBehavior)emptyLinesEnabled );
		else
			lineStringList = line.split( separatingCharacter, (QString::SplitBehavior)emptyLinesEnabled );

		// handle empty lines correctly
		if(lineStringList.count() == 0)
			continue;

		// TODO : read strings (comments) or datetime too
		for ( int n=startColumn; n<=endColumn; n++ ){
// 			if (n<lineStringList.size())
// 				dataPointers[n-startColumn]->insert(i, lineStringList.at(n).toDouble());
// 			else
// 				dataPointers[n-startColumn]->operator[](i) = 0;
			
			Column* column = dataSource->child<Column>(columnOffset+n-startColumn);
			if (n<lineStringList.size())
				column->setValueAt(currentRow, lineStringList.at(n).toDouble());
			else
				column->setValueAt(currentRow, NAN);
		}
		
		currentRow++;
		emit q->completed(100*currentRow/numLines);
    }
    
    //set the comments for each of the columns
    //TODO: generalize to different data types
    QString comment;
	if (headerEnabled)
		comment = "numerical data, " + QString::number(currentRow) + " elements";
	else
		comment = "numerical data, " + QString::number(currentRow+1) + " elements";
	
	for ( int n=startColumn; n<=endColumn; n++ ){
		Column* column = spreadsheet->column(columnOffset+n-startColumn);
// 		column->setUndoAware(false);
		column->setComment(comment);
		column->setUndoAware(true);
	}

	spreadsheet->setUndoAware(true);
	qDebug()<<"file successfully read";
}

/*!
    writes the content of \c dataSource to the file \c fileName.
*/
void AsciiFilterPrivate::write(const QString & fileName, AbstractDataSource* dataSource){
    Q_UNUSED(fileName);
    Q_UNUSED(dataSource);
}


//##############################################################################
//##################  Serialization/Deserialization  ###########################
//##############################################################################
/*!
  Saves as XML.
 */
void AsciiFilter::save(QXmlStreamWriter* writer) const {
    writer->writeStartElement( "asciiFilter" );
	writer->writeAttribute( "commentCharacter", d->commentCharacter );
	writer->writeAttribute( "separatingCharacter", d->separatingCharacter );
	writer->writeAttribute( "autoMode", QString::number(d->autoModeEnabled) );
	writer->writeAttribute( "header", QString::number(d->headerEnabled) );
	writer->writeAttribute( "vectorNames", d->vectorNames );
	writer->writeAttribute( "emptyLines", QString::number(d->emptyLinesEnabled) );
	writer->writeAttribute( "simplifyWhitespaces", QString::number(d->simplifyWhitespacesEnabled) );
	writer->writeAttribute( "transposed", QString::number(d->transposed) );
	writer->writeAttribute( "startRow", QString::number(d->startRow) );
	writer->writeAttribute( "endRow", QString::number(d->endRow) );
	writer->writeAttribute( "startColumn", QString::number(d->startColumn) );
	writer->writeAttribute( "endColumn", QString::number(d->endColumn) );
	writer->writeEndElement();
}

/*!
  Loads from XML.
*/
bool AsciiFilter::load(XmlStreamReader* reader) {
	if(!reader->isStartElement() || reader->name() != "asciiFilter"){
        reader->raiseError(tr("no ascii filter element found"));
        return false;
    }

    QString attributeWarning = tr("Attribute '%1' missing or empty, default value is used");
    QXmlStreamAttributes attribs;
    QString str;

    while (!reader->atEnd()){
        reader->readNext();
        if (reader->isEndElement() && reader->name() == "asciiFilter")
            break;

        if (!reader->isStartElement())
            continue;

		attribs = reader->attributes();

		str = attribs.value("commentCharacter").toString();
		if(str.isEmpty())
			reader->raiseWarning(attributeWarning.arg("'commentCharacter'"));
		else
			d->commentCharacter = str;

		str = attribs.value("separatingCharacter").toString();
		if(str.isEmpty())
			reader->raiseWarning(attributeWarning.arg("'separatingCharacter'"));
		else
			d->separatingCharacter = str;

		str = attribs.value("autoMode").toString();
		if(str.isEmpty())
			reader->raiseWarning(attributeWarning.arg("'autoMode'"));
		else
			d->autoModeEnabled = str.toInt();

		str = attribs.value("header").toString();
		if(str.isEmpty())
			reader->raiseWarning(attributeWarning.arg("'header'"));
		else
			d->headerEnabled = str.toInt();
		
		str = attribs.value("vectorNames").toString();
		if(str.isEmpty())
			reader->raiseWarning(attributeWarning.arg("'vectorNames'"));
		else
			d->vectorNames = str;
		
		str = attribs.value("simplifyWhitespaces").toString();
		if(str.isEmpty())
			reader->raiseWarning(attributeWarning.arg("'simplifyWhitespaces'"));
		else
			d->simplifyWhitespacesEnabled = str.toInt();

		str = attribs.value("transposed").toString();
		if(str.isEmpty())
			reader->raiseWarning(attributeWarning.arg("'transposed'"));
		else
			d->transposed = str.toInt();
		
		str = attribs.value("startRow").toString();
		if(str.isEmpty())
			reader->raiseWarning(attributeWarning.arg("'startRow'"));
		else
			d->startRow = str.toInt();

		str = attribs.value("endRow").toString();
		if(str.isEmpty())
			reader->raiseWarning(attributeWarning.arg("'endRow'"));
		else
			d->endRow = str.toInt();

		str = attribs.value("startColumn").toString();
		if(str.isEmpty())
			reader->raiseWarning(attributeWarning.arg("'startColumn'"));
		else
			d->startColumn = str.toInt();

		str = attribs.value("endColumn").toString();
		if(str.isEmpty())
			reader->raiseWarning(attributeWarning.arg("'endColumn'"));
		else
			d->endColumn = str.toInt();
	}

	return true;
}

// Q_EXPORT_PLUGIN2(ioasciifilter, AsciiFilter)
