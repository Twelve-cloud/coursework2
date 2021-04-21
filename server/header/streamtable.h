#ifndef STREAM_TABLE_H
#define STREAM_TABLE_H

#undef max
#undef min

#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <cstring>

#define CRLF "\n"

class StreamTable
{
public:
    std::ostream &os_;

    StreamTable(std::ostream &os = std::cout, char delimRow = ' ', char delimCol = ' ') :
        os_(os), colIndex_(0), borderExtOn_(true), delimRowOn_(true), delimRow_(delimRow),
        delimColOn_(true), delimCol_(delimCol), firstCell_(1) {}
    virtual ~StreamTable() {}
    virtual std::ostream& os() const { return os_; }

    void MakeRow()
    {
        DelimCol();
        for (int i = 0; i < (int)colWidth_.size(); i++)
        {
            if (visible_[i])
            {
                os_.width(colWidth_[i] + 1);
                os_.fill(delimRow_);
                DelimCol();
            }
        }
    }

    void MakeBorderExt(bool on) { borderExtOn_ = on; } // функция отображения внешних границ

    //функция для задания символа разделителя строк
    void SetDelimRow(bool delimOn, char delimRow = ' ')
    {
        delimRowOn_ = delimOn;
        if (delimRowOn_)
        {
            delimRow_ = delimRow;
        }
        else if (!delimColOn_)
        {
            MakeBorderExt(false);
        }
    }

    //функция для задания символа разделителя столбцов
    void SetDelimCol(bool delimOn, char delimCol = ' ')
    {
        delimColOn_ = delimOn;
        if (delimColOn_)
        {
            delimCol_ = delimCol;
        }
        else if (!delimRowOn_)
        {
            MakeBorderExt(false);
        }
    }

    int AddCol(int colWidth, bool visible = true)
    {
        colWidth_.push_back(colWidth);
        visible_.push_back(visible);
        return colWidth_.back();
    }

    void firstCell(bool isFirst) { firstCell_ = isFirst; }

    void SetCols(int colCount, int colWidth = 0)
    {
        Clear();
        for (int i = 0; i < colCount; i++)
        {
            AddCol(colWidth);
        }
    }

    virtual void Clear()
    {
        colWidth_.clear();
        visible_.clear();
        colIndex_ = 0;
        firstCell_ = 1;
    }

    template <typename T> StreamTable &operator <<(const T &obj)
    {
        Push(obj);
        return *this;
    }

    StreamTable &operator <<(const std::string &s)
    {
        colWidth_[colIndex_] = std::max(colWidth_[colIndex_], (int)s.size() + 1);
        Push(s);
        return *this;
    }

    StreamTable &operator <<(const char *s)
    {
        colWidth_[colIndex_] = std::max(colWidth_[colIndex_], (int)strlen(s) + 1);
        Push(s);
        return *this;
    }


private:
    int colIndex_;
    bool borderExtOn_;
    bool delimRowOn_;
    char delimRow_;
    bool delimColOn_;
    char delimCol_;

    std::vector<int> colWidth_;
    std::vector<int> visible_;
    bool firstCell_;

    template <typename T>
    void Push(const T &obj)
    {
        if (firstCell_)
        {
            if (borderExtOn_)
                MakeRowBorder();

            firstCell_ = 0;
        }

        if (visible_[colIndex_])
        {
            DelimCol();

            os_.width(colWidth_[colIndex_]);
            os_.fill(' ');
            os_ << obj;
        }

        if (++colIndex_ == (int)colWidth_.size())
        {
            DelimCol();

            if (delimRowOn_)
            {
                MakeRowBorder();
            }
            else
            {
                os_ << CRLF;
            }

            colIndex_ = 0;
        }
    }

    void MakeRowBorder()
    {
        os_ << CRLF;
        DelimCol();

        for (int i = 0; i < (int)colWidth_.size(); i++)
        {
            if (visible_[i])
            {
                os_.width(colWidth_[i] + 1);
                os_.fill(delimRow_);
                DelimCol();
            }
        }
        os_ << CRLF;
    }

    void DelimCol()
    {
        if (delimColOn_ && (borderExtOn_ || colIndex_))
        {
            os_ << delimCol_;
        }
        else
        {
            os_ << ' ';
        }
    }

    StreamTable &operator = (const StreamTable &);
};

#endif // STREAM_TABLE_H
