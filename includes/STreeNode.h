#ifndef _STREENODE_H_
#define _STREENODE_H_

#include <iostream>

namespace stree {

namespace infoproviders {

class STreeNodeInfoProvider;

}

class STreeNode {
public:
	STreeNode();
	virtual ~STreeNode();

	virtual void *getValue() = 0;
	virtual void setValue(void *value) = 0;
	virtual infoproviders::STreeNodeInfoProvider *getInfoProvider();

	int getSTreeID();

	void writeSTreeNode(STreeNode *node, std::ostream *stream);
};

}

#endif
