#include <iostream>
#include <cassert>
#include <cstring>

class MedalRow {
    char country[4];
    int* medals;
public:
    int sizeTable;

    static const int GOLD{ 0 };
    static const int SILVER{ 1 };
    static const int BRONZE{ 2 };

    MedalRow(const char* countryP, const int* medalsP, int sizeTable)
        : sizeTable(sizeTable)
    {
        medals = new int[sizeTable];
        std::memset(country, 0, sizeof(country));

        if (countryP) {
            strncpy_s(country, sizeof(country), countryP, _TRUNCATE);
        }

        for (int i = 0; i < sizeTable; ++i) {
            medals[i] = medalsP ? medalsP[i] : 0;
        }
    }

    MedalRow(const MedalRow& other)
        : sizeTable(other.sizeTable)
    {
        medals = new int[sizeTable];
        std::memcpy(country, other.country, sizeof(country));

        for (int i = 0; i < sizeTable; ++i) {
            medals[i] = other.medals[i];
        }
    }

    MedalRow& operator=(const MedalRow& other)
    {
        if (this != &other) {
            delete[] medals;
            sizeTable = other.sizeTable;
            medals = new int[sizeTable];
            std::memcpy(country, other.country, sizeof(country));

            for (int i = 0; i < sizeTable; ++i) {
                medals[i] = other.medals[i];
            }
        }
        return *this;
    }

    ~MedalRow()
    {
        delete[] medals;
    }

    MedalRow() : MedalRow(nullptr, nullptr, 10) {}

    MedalRow& setCountry(const char* countryP)
    {
        if (countryP) {
            strncpy_s(country, sizeof(country), countryP, _TRUNCATE);
        }
        return *this;
    }

    const char* getCountry() const { return country; }

    int& operator[](int idx)
    {
        assert((idx >= 0 and idx < sizeTable) and "Index out of range!");
        return medals[idx];
    }

    int operator[](int idx) const
    {
        assert((idx >= 0 and idx < sizeTable) and "Index out of range!");
        return medals[idx];
    }

    friend std::ostream& operator<<(std::ostream& os, const MedalRow& row)
    {
        os << '[' << row.country << "]-( ";
        for (int i = 0; i < row.sizeTable; ++i) {
            os << row.medals[i];
            if (i < row.sizeTable - 1) { os << '\t'; }
        }
        os << " )";
        return os;
    }
};

class MedalsTable {
    MedalRow* rows;
    int numRows;

public:
    MedalsTable(int numRows, int sizeTable)
        : numRows(numRows)
    {
        rows = new MedalRow[numRows];

        for (int i = 0; i < numRows; ++i) {
            rows[i] = MedalRow(nullptr, nullptr, sizeTable);
        }
    }

    ~MedalsTable()
    {
        delete[] rows;
    }

    MedalRow& operator[](int idx)
    {
        assert((idx >= 0 and idx < numRows) and "Index out of range!");
        return rows[idx];
    }

    MedalRow operator[](int idx) const
    {
        assert((idx >= 0 and idx < numRows) and "Index out of range!");
        return rows[idx];
    }

    friend std::ostream& operator<<(std::ostream& os, const MedalsTable& table)
    {
        for (int i = 0; i < table.numRows; ++i) {
            os << table.rows[i] << '\n';
        }
        return os;
    }

    int operator()(const char* country) const
    {
        for (int i = 0; i < numRows; ++i) {
            if (std::strcmp(rows[i].getCountry(), country) == 0) {
                return rows[i].sizeTable;
            }
        }
        return 0;
    }
};

int main()
{
    MedalsTable table(2, 8);

    table[0].setCountry("USA");
    table[0][MedalRow::GOLD] = 5;
    table[0][MedalRow::SILVER] = 3;
    table[0][MedalRow::BRONZE] = 2;

    table[1].setCountry("UK");
    table[1][MedalRow::GOLD] = 3;
    table[1][MedalRow::SILVER] = 4;
    table[1][MedalRow::BRONZE] = 1;

    std::cout << table << '\n';

    std::cout << "Max medals for USA: " << table("USA") << '\n';

    return 0;
}
