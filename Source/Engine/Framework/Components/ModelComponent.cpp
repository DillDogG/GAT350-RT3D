#include "ModelComponent.h"
#include "Framework/Actor.h"
#include "Framework/Resource/ResourceManager.h"

namespace nc {
	CLASS_DEFINITION(ModelComponent)

	bool ModelComponent::Initialize() {
		if (!modelName.empty()) {
			m_model = GET_RESOURCE(Model, modelName);
			
		}
		if (m_model && !materialName.empty()) {
			m_material = (GET_RESOURCE(Material, materialName));
		}

		return true;
	}

	void ModelComponent::Update(float dt)
	{
	}

	void ModelComponent::Draw(Renderer& renderer) {
		m_material->Bind();
		m_material->GetProgram()->SetUniform("model", m_owner->transform.GetMatrix());

		glDepthMask(enableDepth);
		glCullFace(cullface);

		m_model->Draw();
		//m_model->Draw(renderer, m_owner->transform);
	}

	void nc::ModelComponent::ProcessGui() {
		ImGui::Checkbox("Cast Shadow?", &castShadow);
		ImGui::Checkbox("Enable Depth", &enableDepth);
	}

	void ModelComponent::Read(const json_t& value) {
		READ_DATA(value, modelName);
		READ_DATA(value, materialName);

		READ_DATA(value, enableDepth);
		READ_DATA(value, castShadow);

		std::string cullfaceName;
		if (READ_NAME_DATA(value, "cullface", cullfaceName)) {
			if (IsEqualIgnoreCase(cullfaceName, "front")) cullface = GL_FRONT;
		}
	}
}
