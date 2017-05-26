#include "pdfprinter.h"

#include <QPrinter>
#include <QPainter>
#include <QTextDocument>
#include <QFile>
#include <QString>
#include <QByteArray>

#include <string>
#include <regex>

void PDFPrinter::init(Transducer* transducer)
{
    dict_.emplace(std::make_pair(std::string("bvd_series"),std::string(":/icons/bvd-series.svg")));
    dict_.emplace(std::make_pair(std::string("bvd_parallel"),std::string(":/icons/bvd-parallel.svg")));
    dict_.emplace(std::make_pair(std::string("transducer_name"),transducer->get_name()));
}

std::string PDFPrinter::replace_words(const std::string& str) const
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

int PDFPrinter::print(Transducer *transducer, const std::string& path, PDFOptions options)
{
    init(transducer);
    ///TODO: add checking whether file is writeable

    QPrinter file_printer;
    file_printer.setOutputFileName(path.c_str());
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

    /*QFont header_font(QFont("Times",22,QFont::Bold),&file_printer);
    painter.setFont(header_font);
    painter.drawText(file_printer.pageRect(),"Przetwornik 2_P0",QTextOption(Qt::AlignHCenter));
    painter.end();*/

    return 0;
}
