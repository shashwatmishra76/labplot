/***************************************************************************
    File                 : PlotDataDialog.h
    Project              : LabPlot
    Description          : Dialog for generating plots for the spreadsheet data
    --------------------------------------------------------------------
    Copyright            : (C) 2017 by Alexander Semke (alexander.semke@web.de)

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
#ifndef PLOTDATADIALOG_H
#define PLOTDATADIALOG_H

#include "ui_plotdatawidget.h"

#include <QDialog>

class QComboBox;
class Spreadsheet;
class TreeViewComboBox;

class PlotDataDialog : public QDialog {
	Q_OBJECT

	public:
		explicit PlotDataDialog(Spreadsheet*, QWidget* parent = 0, Qt::WFlags fl = 0);
		~PlotDataDialog();

	private:
		Ui::PlotDataWidget ui;
		Spreadsheet* m_spreadsheet;
		TreeViewComboBox* cbExistingPlots;
		TreeViewComboBox* cbExistingWorksheets;
		QList<QComboBox*> m_columnComboBoxes;

		void processColumns();

	private slots:
		void plot();
};

#endif
