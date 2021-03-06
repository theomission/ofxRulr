#pragma once

#include "ofxCvGui/Element.h"

namespace ofxRulr {
	namespace Nodes {
		class Base;
	}

	namespace Graph {
		namespace Editor {
			/// A class to draw a preview icon for a Node type
			class PinView : public ofxCvGui::Element {
			public:
				PinView();

				void setup(Nodes::Base & node);

				template<typename NodeType>
				void setup() {
					NodeType tempNode;
					this->setup(tempNode);
				}
			protected:
				shared_ptr<ofImage> icon;
				string nodeTypeName;
			};
		}
	}
}