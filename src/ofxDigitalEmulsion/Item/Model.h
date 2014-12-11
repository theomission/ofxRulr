#pragma once

#include "Base.h"

#include "ofxAssimpModelLoader.h"

namespace ofxDigitalEmulsion {
	namespace Item {
		class Model : public Base {
		public:
			Model();
			string getTypeName() const override;
			void init() override;
			ofxCvGui::PanelPtr getView();
			void drawWorld() override;

			void serialize(Json::Value &) override;
			void deserialize(const Json::Value &) override;
		protected:
			void populateInspector2(ofxCvGui::ElementGroupPtr) override;

			void updatePreviewMesh();

			ofxCvGui::PanelPtr view;

			ofParameter<string> filename;

			ofParameter<bool> drawVertices, drawWireframe, drawFaces;

			ofParameter<bool> flipX, flipY, flipZ;
			ofParameter<float> inputUnitScale;

			ofxAssimpModelLoader modelLoader;
		};
	}
}