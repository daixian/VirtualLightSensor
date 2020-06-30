#include "Profile.h"
#include "CVSystem/CVSystem.h"

namespace dxlib {

Profile* Profile::m_pInstance = NULL;

Profile::Profile()
{
}

Profile::~Profile()
{
}

Profile* Profile::GetInst()
{
    if (m_pInstance == NULL)
        m_pInstance = new Profile();
    return m_pInstance;
}

void Profile::load(std::string path)
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