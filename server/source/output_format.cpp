#include "header/streamtable.h"

extern StreamTable serverTable;
extern StreamTable serverConfigTable;
extern StreamTable mysqlConfigTable;
extern StreamTable selectTable;

void setOutputFormat()
{
    serverTable.AddCol(50);
    serverTable.MakeBorderExt(true);
    serverTable.SetDelimRow(true, '-');
    serverTable.SetDelimCol(true, '|');

    serverConfigTable.AddCol(40);
    serverConfigTable.AddCol(40);
    serverConfigTable.MakeBorderExt(true);
    serverConfigTable.SetDelimRow(true, '-');
    serverConfigTable.SetDelimCol(true, '|');

    mysqlConfigTable.AddCol(40);
    mysqlConfigTable.AddCol(40);
    mysqlConfigTable.MakeBorderExt(true);
    mysqlConfigTable.SetDelimRow(true, '-');
    mysqlConfigTable.SetDelimCol(true, '|');

    selectTable.AddCol(20);
    selectTable.AddCol(20);
    selectTable.AddCol(20);
    selectTable.AddCol(20);
    selectTable.AddCol(20);
    selectTable.AddCol(20);
    selectTable.MakeBorderExt(true);
    selectTable.SetDelimRow(true, '-');
    selectTable.SetDelimCol(true, '|');
}
