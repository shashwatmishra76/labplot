/***************************************************************************
File                 : NetCDFFilterPrivate.h
Project              : LabPlot
Description          : Private implementation class for NetCDFFilter.
--------------------------------------------------------------------
Copyright            : (C) 2015 Stefan Gerlach (stefan.gerlach@uni.kn)
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
#ifndef NETCDFFILTERPRIVATE_H
#define NETCDFFILTERPRIVATE_H

#ifdef HAVE_NETCDF
#include <netcdf.h>
#endif

class AbstractDataSource;

class NetCDFFilterPrivate {

	public:
		explicit NetCDFFilterPrivate(NetCDFFilter*);

		void parse(const QString & fileName, QTreeWidgetItem* rootItem);
		void read(const QString & fileName, AbstractDataSource* dataSource,
					AbstractFileFilter::ImportMode importMode = AbstractFileFilter::Replace);
		QString readAttribute(const QString & fileName, const QString & name, const QString & varName);
		QString readCurrentVar(const QString & fileName, AbstractDataSource* dataSource, AbstractFileFilter::ImportMode importMode=AbstractFileFilter::Replace, int lines=-1);
		void write(const QString & fileName, AbstractDataSource* dataSource);

		const NetCDFFilter* q;

		QString currentVarName;
		int startRow;
		int endRow;
		int startColumn;
		int endColumn;
		bool autoModeEnabled;

	private:
		int status;
		//const static int MAXNAMELENGTH=1024;
		//QList<unsigned long> multiLinkList;	// used to find hard links
#ifdef HAVE_NETCDF
		void handleError(int status, QString function);
		QString translateDataType(nc_type type);
		QString scanAttrs(int ncid, int varid, int attid, QTreeWidgetItem* parentItem=NULL);
		void scanDims(int ncid, int ndims, QTreeWidgetItem* parentItem);
		void scanVars(int ncid, int nvars, QTreeWidgetItem* parentItem);
/*		QString translateHDFOrder(H5T_order_t);
		QString translateHDFType(hid_t);
		QString translateHDFClass(H5T_class_t);
		QStringList readHDFCompound(hid_t tid);
		template <typename T> QStringList readHDFData1D(hid_t dataset, hid_t type, int rows, int lines, QVector<double> *dataPointer=NULL);
		QStringList readHDFCompoundData1D(hid_t dataset, hid_t tid, int rows, int lines,QVector< QVector<double>* >& dataPointer);
		template <typename T> QStringList readHDFData2D(hid_t dataset, hid_t ctype, int rows, int cols, int lines, QVector< QVector<double>* >& dataPointer);
		QStringList readHDFCompoundData2D(hid_t dataset, hid_t tid, int rows, int cols, int lines);
		QStringList readHDFAttr(hid_t aid);
		QStringList scanHDFAttrs(hid_t oid);
		QStringList readHDFDataType(hid_t tid);
		QStringList readHDFPropertyList(hid_t pid);
		void scanHDFDataType(hid_t tid, char *dataTypeName,  QTreeWidgetItem* parentItem);
		void scanHDFLink(hid_t gid, char *linkName,  QTreeWidgetItem* parentItem);
		void scanHDFDataSet(hid_t dsid, char *dataSetName,  QTreeWidgetItem* parentItem);
		void scanHDFGroup(hid_t gid, char *groupName, QTreeWidgetItem* parentItem);
*/
#endif
};

#endif