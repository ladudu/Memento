////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2021 Ripose
//
// This file is part of Memento.
//
// Memento is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, version 2 of the License.
//
// Memento is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Memento.  If not, see <https://www.gnu.org/licenses/>.
//
////////////////////////////////////////////////////////////////////////////////

#include "optionswindow.h"
#include "ui_optionswindow.h"

#include "ankisettings.h"
#include "audiosourcesettings.h"
#include "behaviorsettings.h"
#include "dictionarysettings.h"
#include "interfacesettings.h"
#ifdef OCR_SUPPORT
#include "ocrsettings.h"
#endif // OCR_SUPPORT
#include "searchsettings.h"

/* Begin Constructor/Destructor */

OptionsWindow::OptionsWindow(Context *context, QWidget *parent) :
    QWidget(parent),
    m_ui(std::make_unique<Ui::OptionsWindow>()),
    m_context(std::move(context))
{
    m_ui->setupUi(this);

    QFont labelFont = m_ui->labelOption->font();
#if defined(Q_OS_MACOS)
    labelFont.setPointSize(20);
#else
    labelFont.setPointSize(14);
#endif
    m_ui->labelOption->setFont(labelFont);

    addOption(tr("Anki Integration"), new AnkiSettings(m_context));
    addOption(tr("Audio Sources"), new AudioSourceSettings(m_context));
    addOption(tr("Behavior"), new BehaviorSettings(m_context));
    addOption(tr("Dictionaries"), new DictionarySettings(m_context));
    addOption(tr("Search"), new SearchSettings(m_context));
    addOption(tr("Interface"), new InterfaceSettings(m_context));
#ifdef OCR_SUPPORT
    addOption(tr("OCR"), new OCRSettings(m_context));
#endif // OCR_SUPPORT

    connect(
        m_ui->listOptions, &QListWidget::itemSelectionChanged,
        this, &OptionsWindow::showSelectedOption
    );
}

OptionsWindow::~OptionsWindow()
{

}

/* End Constructor/Destructor */
/* Begin Event Handlers */

void OptionsWindow::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    m_ui->listOptions->setCurrentRow(0);
    showSelectedOption();
}

void OptionsWindow::changeEvent(QEvent *event)
{
    QWidget::changeEvent(event);
    if (event->type() == QEvent::LanguageChange)
    {
        m_ui->retranslateUi(this);
        retranslateOptionList();
    }
}

/* End Event Handlers */
/* Begin Helpers */

void OptionsWindow::addOption(const QString &name, QPointer<QWidget> widget)
{
    m_ui->listOptions->addItem(name);
    widget->hide();
    m_ui->layoutWidgets->addWidget(widget);
    m_widgets.append(widget);
}

void OptionsWindow::showSelectedOption()
{
    int currentRow = m_ui->listOptions->currentRow();

    if (currentRow < 0 || currentRow >= m_widgets.size())
    {
        return;
    }

    if (m_currentWidget)
        m_currentWidget->hide();

    QWidget *widget = m_widgets[currentRow];
    if (widget)
    {
        widget->show();
        m_currentWidget = widget;

        QListWidgetItem *item = m_ui->listOptions->item(currentRow);
        if (item)
        {
            m_ui->labelOption->setText(item->text());
        }
    }
    else if (m_currentWidget)
    {
        m_currentWidget->show();
    }
}

void OptionsWindow::retranslateOptionList()
{
    return; // Temporarily disable to investigate issues
    /* Save current selection */
    int currentRow = m_ui->listOptions->currentRow();

    /* Update list item texts - must match the order in constructor */
    QStringList optionNames;
    optionNames << tr("Anki Integration")
                << tr("Audio Sources")
                << tr("Behavior")
                << tr("Dictionaries")
                << tr("Search")
                << tr("Interface");
#ifdef OCR_SUPPORT
    optionNames << tr("OCR");
#endif

    for (int i = 0; i < m_ui->listOptions->count() && i < optionNames.size(); ++i)
    {
        QListWidgetItem *item = m_ui->listOptions->item(i);
        if (item)
        {
            item->setText(optionNames[i]);
        }
    }

    /* Restore selection */
    if (currentRow >= 0 && currentRow < m_ui->listOptions->count())
    {
        /* Block signals to prevent triggering itemSelectionChanged during language change */
        bool oldState = m_ui->listOptions->blockSignals(true);
        m_ui->listOptions->setCurrentRow(currentRow);
        m_ui->listOptions->blockSignals(oldState);

        QListWidgetItem *item = m_ui->listOptions->item(currentRow);
        if (item)
        {
            m_ui->labelOption->setText(item->text());
        }
    }
}

/* End Helpers */
