#include "NoteBook.h"
#include "CVSystem/CVSystem.h"

namespace dxlib {

NoteBook* NoteBook::m_pInstance = NULL;

NoteBook::NoteBook()
{
}

NoteBook::~NoteBook()
{
}

NoteBook* NoteBook::GetInst()
{
    if (m_pInstance == NULL)
        m_pInstance = new NoteBook();
    return m_pInstance;
}

void NoteBook::load(std::string path)
{
    using namespace xuexue::json;
    std::ifstream ifs;
    ifs.open(path);
    if (ifs.is_open()) {
        JsonMapper::toObject(ifs, *this);
        //此时不能写log
    }
    ifs.close();
}

} // namespace dxlib