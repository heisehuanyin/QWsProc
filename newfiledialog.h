#ifndef NEWFILEDIALOG_H
#define NEWFILEDIALOG_H

#include "wscore.h"
#include <QWidget>
#include <QWizard>

namespace Core {

    class NewFileDialog : public QWizard
    {
        Q_OBJECT
        NewFileDialog(){}
    public:
        virtual ~NewFileDialog() override{}

        static void xx();


    };
}

#endif // NEWFILEDIALOG_H
