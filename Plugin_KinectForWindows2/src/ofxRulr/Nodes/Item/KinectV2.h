#pragma once

#include "ofxRulr/Nodes/Item/RigidBody.h"

#include "ofxKinectForWindows2.h"

#include "ofxCvGui/Panels/Groups/Grid.h"

#ifdef  Plugin_KinectForWindows2_Exports 
	#define DLLEXPORT __declspec(dllexport)  
#else
	#define DLLEXPORT __declspec(dllimport)  
#endif

namespace ofxRulr {
	namespace Nodes {
		namespace Item {
			class DLLEXPORT KinectV2 : public ofxRulr::Nodes::Item::RigidBody {
			public:
				KinectV2();
				void init();
				string getTypeName() const override;
				void update();
				ofxCvGui::PanelPtr getView() override;

				void serialize(Json::Value &);
				void deserialize(const Json::Value &);

				void drawObject() override;
				shared_ptr<ofxKinectForWindows2::Device> getDevice();

			protected:
				void populateInspector(ofxCvGui::ElementGroupPtr);
				shared_ptr<ofxKinectForWindows2::Device> device;
				shared_ptr<ofxCvGui::Panels::Groups::Grid> view;

				ofParameter<int> playState;
				ofParameter<int> viewType;
			};
		}
	}
}