/***************************************************************************
    File                 : MatrixDock.h
    Project              : LabPlot
    Description          : widget for matrix properties
    --------------------------------------------------------------------
    Copyright            : (C) 2015 by Alexander Semke (alexander.semke@web.de)

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

#ifndef MATRIXDOCK_H
#define MATRIXDOCK_H

#include <QtGui/QWidget>
#include "ui_matrixdock.h"

class Matrix;
class AbstractAspect;

class MatrixDock: public QWidget {
	Q_OBJECT

public:
	explicit MatrixDock(QWidget*);
	void setMatrices(QList<Matrix*>);

private:
	Ui::MatrixDock ui;
	QList<Matrix*> m_matrixList;
	Matrix* m_matrix;
	bool m_initializing;

private slots:
	//SLOTs for changes triggered in MatrixDock
	void nameChanged();
	void commentChanged();
	void rowCountChanged(int);
	void columnCountChanged(int);

	//SLOTs for changes triggered in Matrix
	void matrixDescriptionChanged(const AbstractAspect*);
	void matrixRowCountChanged(int);
	void matrixColumnCountChanged(int);

	void loadConfigFromTemplate(KConfig&);
	void loadConfig(KConfig&);
	void saveConfig(KConfig&);

signals:
	void info(const QString&);
};

#endif // MATRIXDOCK_H