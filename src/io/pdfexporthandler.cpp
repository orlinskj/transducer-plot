#include "pdfexporthandler.h"

#include "../viewmodel/plotitem.h"
#include "../viewmodel/functionitem.h"
#include "../viewmodel/transduceritem.h"
#include "../view/plotpresenter.h"
#include "../viewmodel/plotstoreitemmodel.h"
#include "../model/solver.h"

#include <QPrinter>
#include <QFile>
#include <QTextDocument>
#include <QTextCursor>
#include <regex>

PDFExportHandler::PDFExportHandler(
        TransducerItem* transducer,
        const PDFExportHandler::Options& options,
        BVDSolver* solver,
        PlotPresenter* presenter
        ) :
    ExportFileHandler(transducer, "pdf"), options_(options), solver_(solver), presenter_(presenter)
{ }

void PDFExportHandler::add_hook(const std::string& hook, const std::string& val)
{
    dict_.emplace(std::make_pair(hook,val));
}

void PDFExportHandler::add_hook(const std::string& hook, double val){
    dict_.emplace(std::make_pair(hook, QString::number(val).toStdString()));
}

std::string PDFExportHandler::replace_words(const std::string& str) const
{
    std::string s = str;
    std::smatch m;
    std::regex re("\\{\\{(\\w+)\\}\\}");

    while ( std::regex_search(s,m,re) )
    {
        auto it = dict_.find(m[1].str());
        if (it != dict_.end())
            s.replace(m.position(),m.length(),it->second);
        else
            s.replace(m.position(),m.length(),m[1].str());
    }

    return s;
}

int PDFExportHandler::save(const std::string &filepath)
{
    auto transducer = transducer_->value();

    // ----------------- init --------------------
    add_hook("bvd_series",":/icons/bvd-series.svg");
    add_hook("bvd_parallel",":/icons/bvd-parallel.svg");
    add_hook("transducer_name", transducer->get_name());

    // series
    add_hook("fs",  solver_->f(BVDSolver::Type::Series));
    add_hook("Cos", solver_->Co(BVDSolver::Type::Series));
    add_hook("Rs",  solver_->R(BVDSolver::Type::Series));
    add_hook("Ls",  solver_->L(BVDSolver::Type::Series));
    add_hook("Cs",  solver_->C(BVDSolver::Type::Series));
    add_hook("Qs",  solver_->Q(BVDSolver::Type::Series));

    // parallel
    add_hook("fp",  solver_->f(BVDSolver::Type::Parallel));
    add_hook("Cop", solver_->Co(BVDSolver::Type::Parallel));
    add_hook("Rp",  solver_->R(BVDSolver::Type::Parallel));
    add_hook("Lp",  solver_->L(BVDSolver::Type::Parallel));
    add_hook("Cp",  solver_->C(BVDSolver::Type::Parallel));
    add_hook("Qp",  solver_->Q(BVDSolver::Type::Parallel));

    // common
    add_hook("Q",   solver_->QQ());
    add_hook("k33", solver_->k33());
    add_hook("keff", solver_->keff());
    add_hook("k", solver_->k());

    // ------------------ printing ------------------
    QTextDocument doc;
    QTextCursor cursor(&doc);

    QPrinter file_printer;
    qDebug() << "DPI: " << file_printer.resolution();
    file_printer.setResolution(120);
    file_printer.setOutputFileName(QString::fromStdString(this->path(filepath)));

    doc.setDefaultStyleSheet("* { font-family: 'Arial'; font-size: 18px; }");
    doc.setPageSize(file_printer.pageRect().size());


    std::string html;
    QFile pdf_layout(":/pdf_layout.html");
    if (!pdf_layout.open(QIODevice::ReadOnly | QIODevice::Text))
        return -1;

    while (!pdf_layout.atEnd())
    {
        std::string line = pdf_layout.readLine().toStdString();
        line = replace_words(line);
        html += line;
    }

    pdf_layout.close();

    doc.setHtml(QString::fromStdString(html));
    cursor.movePosition(QTextCursor::End);

    // ----------------- inserting plots -----------------
    if (options_.plots){
        QSize img_size(900,650);

        // plot backup
        auto start_plot = presenter_->plot();
        auto broom_plot = presenter_->broom()->plot();
        auto freq = transducer_->value()->get_set(Unit::Frequency);

        for (const auto& set : transducer_->value()->get_sets()){
            if (set.unit() == Unit::Frequency){
                continue;
            }

            PlotItem* plot = new PlotItem;
            presenter_->store()->append(plot);
            presenter_->show_plot(plot);

            // Function and FunctionItem are deleted during removing from parent
            auto func_item = new FunctionItem(new Function(transducer_->value(),freq, &set));
            plot->append(func_item);

            // draw
            QImage img = presenter_->screenshot(img_size.width(), img_size.height());
            cursor.insertImage(img);
            cursor.insertHtml("<br><br>");

            // clean
            presenter_->store()->remove(plot);
        }

        // setting back plot
        presenter_->show_plot(start_plot);
        //presenter_->store()->remove(plot);
    }

    doc.print(&file_printer);

    return 0;
}
