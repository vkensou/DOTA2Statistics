#include "heroitemsmanager.h"
#include "dataconfig.h"

HeroItemsManager::HeroItemsManager()
{
}

HeroItems &HeroItemsManager::getHeroItems(const QString &name, const DataConfig &config)
{
    static QString keyfmt = "%1%2";
    QString key = keyfmt.arg(name).arg(config.getFileParams());

    auto i = list.find(key);
    if(i == list.end())
    {
        i = list.insert(key, {name});
        (*i).load();
    }
    return *i;
}
