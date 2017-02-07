#ifndef _STREENODEINFOPROVIDER_H_
#define _STREENODEINFOPROVIDER_H_

#include <iostream>

namespace stree {

class STreeNode;

namespace infoproviders {

class STreeNodeInfoProvider {
public:
	STreeNodeInfoProvider();
	virtual ~STreeNodeInfoProvider();

	virtual void write(STreeNode *node, std::ostream *stream) = 0;
	virtual STreeNode *read(std::istream *stream) = 0;
	virtual char getSTreeID() = 0;
};

}

}

#endif
