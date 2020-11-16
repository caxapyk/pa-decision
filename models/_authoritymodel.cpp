#include "_authoritymodel.h"

_AuthorityModel::_AuthorityModel(QObject *parent) : RefBaseModel(parent)
{
    setQuery("SELECT name, comment, id FROM pad_authority ORDER BY name ASC");
    setRootText(tr("All public authorities"));

    setHeaderData(0, Qt::Horizontal, tr("Public authorities"));
}
