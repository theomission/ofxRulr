#pragma once

#include "../Graph/Pin.h"
#include "../Utils/Constants.h"
#include "../Utils/Serializable.h"
#include "../Exception.h"

#include "../../../addons/ofxCvGui/src/ofxCvGui/InspectController.h"

#include "ofImage.h"
#include "ofxAssets.h"

#include <string>

#define RULR_NODE_INIT_LISTENER \
	this->onInit += [this]() { \
		this->init(); \
	}
#define RULR_NODE_UPDATE_LISTENER \
	this->onUpdate += [this]() { \
		this->update(); \
	}
#define RULR_NODE_INSPECTOR_LISTENER \
	this->onInspect += [this](ofxCvGui::InspectArguments & args) { \
		this->populateInspector(args.inspector); \
	}
#define RULR_NODE_SERIALIZATION_LISTENERS \
	this->onSerialize += [this](Json::Value & json) { \
		this->serialize(json); \
	}; \
	this->onDeserialize += [this](Json::Value const & json) { \
		this->deserialize(json); \
	}

namespace ofxRulr {
	namespace Nodes {
		class Base : public ofxCvGui::IInspectable, public Utils::Serializable {
		public:
			Base();
			~Base();
			virtual string getTypeName() const override;
			void init();
			void destroy();
			void update();
			string getName() const override;
			void setName(const string);

			ofImage & getIcon();
			const ofColor & getColor();

			const Graph::PinSet & getInputPins() const;
			void populateInspector(ofxCvGui::ElementGroupPtr);
			virtual ofxCvGui::PanelPtr getView() { return ofxCvGui::PanelPtr(); };

			///override this function for any node which can draw to the world
			virtual void drawWorld() { }
			///override this function to specifically define how the stencil of this layer will be drawn
			virtual void drawStencil() {
				this->drawWorld();
			}

			template<typename NodeType>
			void connect(shared_ptr<NodeType> node) {
				auto inputPin = this->getInputPins().get<Pin<NodeType>>();
				if (inputPin) {
					inputPin->connect(node);
				}
				else {
					RULR_ERROR << "Couldn't connect node of type '" << NodeType().getTypeName() << "' to node '" << this->getTypeName() << "'. No matching pin found.";
				}
			}

			template<typename NodeType>
			shared_ptr<NodeType> getInput() const {
				auto pin = this->getInputPin<NodeType>();
				if (pin) {
					return pin->getConnection();
				}
				else {
					return shared_ptr<NodeType>();
				}
			}

			template<typename NodeType>
			shared_ptr<Graph::Pin<NodeType>> getInputPin() const {
				return this->getInputPins().get<Graph::Pin<NodeType>>();
			}

			template<typename NodeType>
			void throwIfMissingAConnection() const {
				if (!this->getInput<NodeType>()) {
					stringstream message;
					message << "Node [" << this->getTypeName() << "] is missing a connection to [" << NodeType().getTypeName() << "]";
					throw(Exception(message.str()));
				}
			}
			void throwIfMissingAnyConnection() const;

			ofxLiquidEvent<void> onInit;
			ofxLiquidEvent<void> onDestroy;
			ofxLiquidEvent<void> onUpdate;

			ofxLiquidEvent<shared_ptr<Graph::AbstractPin>> onConnect;
			ofxLiquidEvent<shared_ptr<Graph::AbstractPin>> onDisconnect;
			ofxLiquidEvent<void> onAnyInputConnectionChanged;

		protected:
			void addInput(shared_ptr<Graph::AbstractPin>);

			template<typename NodeType>
			shared_ptr<Graph::Pin<NodeType>> addInput() {
				auto inputPin = make_shared<Pin<NodeType>>();
				this->addInput(inputPin);
				return inputPin;
			}

			template<typename NodeType>
			shared_ptr<Graph::Pin<NodeType>> addInput(const string & pinName) {
				auto inputPin = make_shared<Graph::Pin<NodeType>>(pinName);
				this->addInput(inputPin);
				return inputPin;
			}

			void removeInput(shared_ptr<Graph::AbstractPin>);
			void clearInputs();

			void setupGraphics();
			void setIcon(ofImage &);
		private:
			Graph::PinSet inputPins;
			ofImage * icon;
			ofColor color;
			string name;
			string defaultIconName;
			bool initialized;
		};
	}
}