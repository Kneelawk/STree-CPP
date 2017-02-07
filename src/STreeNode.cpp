#include "STreeNode.h"
#include "infoproviders/STreeNodeInfoProvider.h"

namespace stree {

STreeNode::STreeNode() {

}

STreeNode::~STreeNode() {

}

int STreeNode::getSTreeID() {
	return getInfoProvider()->getSTreeID();
}

}
