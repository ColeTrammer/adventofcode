// These are needed since the di library is missing these includes in the headers. Not including these causes
// unfortunate compile errors.
#include <di/container/algorithm/compare.h>
#include <di/container/algorithm/equal.h>
#include <di/container/iterator/iterator_extension.h>
#include <di/container/iterator/sentinel_extension.h>
#include <di/container/tree/owning_rb_tree.h>
