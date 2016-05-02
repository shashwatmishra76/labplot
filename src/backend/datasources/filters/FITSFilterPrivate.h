#ifndef FITSFILTERPRIVATE_H
#define FITSFILTERPRIVATE_H

/***************************************************************************
File                 : FITSFilterPrivate.cpp
Project              : LabPlot
Description          : FITS I/O-filter
--------------------------------------------------------------------
Copyright            : (C) 2016 by Fabian Kristof (fkristofszabolcs@gmail.com)
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
#ifdef HAVE_FITS
#include "fitsio.h"
#endif
class AbstractDataSource;

class FITSFilterPrivate {

        public:
                explicit FITSFilterPrivate(FITSFilter*);
                void readCHDU(const QString & fileName, AbstractDataSource* dataSource,
                                        AbstractFileFilter::ImportMode importMode = AbstractFileFilter::Replace);
                void writeCHDU(const QString & fileName, AbstractDataSource* dataSource);

                const FITSFilter* q;
                QStringList extensionNames() const;


        private:
                int status;

#ifdef HAVE_FITS
                fitsfile* fitsFile;
#endif

};

#endif // FITSFILTERPRIVATE_H
