#include "pdfexporthandler.h"

#include "../viewmodel/transduceritem.h"
#include "../model/solver.h"

#include <QPrinter>
#include <QFile>
#include <QTextDocument>
#include <regex>

PDFExportHandler::PDFExportHandler(
        TransducerItem* transducer,
        const PDFExportHandler::Options& options,
        BVDSolver* solver
        ) :
    ExportFileHandler(transducer, "pdf"), options_(options), solver_(solver)
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

    QPrinter file_printer;
    file_printer.setOutputFileName(QString::fromStdString(this->path(filepath)));
    //QPainter painter(&file_printer);

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

    QTextDocument doc;
    doc.setDefaultStyleSheet("* { font-family: 'Times'; }");
    doc.setHtml(QString::fromStdString(html));

    doc.print(&file_printer);

    return 0;
}
