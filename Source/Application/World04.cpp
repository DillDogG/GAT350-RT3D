#include "World04.h"
#include "Framework/Framework.h"
#include "Input/InputSystem.h"
#include <glm/glm/gtc/type_ptr.hpp>
#include <glm/glm/gtx/color_space.hpp>

namespace nc {
    bool World04::Initialize() {
        
        auto material = GET_RESOURCE(Material, "materials/grid.mtrl");
        m_model = std::make_shared<Model>();
        m_model->SetMaterial(material);
        m_model->Load("models/plane.obj", glm::vec3{ 0 }, glm::vec3{ -90, 0, 0 });
        //auto material = GET_RESOURCE(Material, "materials/squirrel.mtrl");
        //m_model = std::make_shared<Model>();
        //m_model->SetMaterial(material);
        //m_model->Load("models/squirrel.glb", glm::vec3{ 0, -0.7f, 0 }, glm::vec3{ 0 }, glm::vec3{ 0.4f });

        //for (int i = 0; i < 3; i++) {
        //    m_lights[i].type = light_t::eType::Point;
        //    m_lights[i].position = glm::vec3{ randomf(-5, 5), randomf(-5, 5), randomf(-5, 5) };
        //    m_lights[i].direction = glm::vec3{ 0, -1, 0 };
        //    m_lights[i].color = glm::rgbColor(glm::vec3{ randomf() * 360, 1, 1 });
        //    m_lights[i].intensity = 1;
        //    m_lights[i].range = 12;
        //    m_lights[i].innerAngle = 10.0f;
        //    m_lights[i].outerAngle = 30.0f;
        //}
        m_light.type = light_t::eType::Point;
        m_light.position = glm::vec3{ 0, 8, 0 };
        m_light.direction = glm::vec3{ 0, -1, 0 };
        //m_light.color = glm::vec3{ 1, 1, 1 };
        m_light.color = glm::rgbColor(glm::vec3{ randomf() * 360, 1, 1 });
        m_light.intensity = 1;
        m_light.range = 12;
        m_light.innerAngle = 10.0f;
        m_light.outerAngle = 30.0f;

        return true;
    }

    void World04::Shutdown() {
    }

    void World04::Update(float dt) {
        ENGINE.GetSystem<Gui>()->BeginFrame();
        ImGui::Begin("Transform");
        ImGui::DragFloat3("Position", &m_transform.position[0], 0.1f);
        ImGui::DragFloat3("Rotation", &m_transform.rotation[0], 0.1f);
        ImGui::DragFloat3("Scale", &m_transform.scale[0], 0.1f);
        ImGui::End();

        ImGui::Begin("Scene");
        ImGui::ColorEdit3("Ambient Color", glm::value_ptr(ambientLight));
        ImGui::Separator();
        
        for (int i = 0; i < 3; i++)
        {
            std::string name = "light" + std::to_string(i);
            if (ImGui::Selectable(name.c_str(), m_selected == i)) m_selected = i;
        }
        ImGui::End(); 

        ImGui::Begin("Light");
        const char* types[] = {"Point", "Directional", "Spot"};
        ImGui::Combo("Type", (int*)(& m_light.type), types, 3);
        ImGui::DragFloat3("Ambient Light", glm::value_ptr(ambientLight), 0.1f);
        ImGui::DragFloat3("Diffuse Light", glm::value_ptr(m_light.color), 0.1f);
        if (m_light.type != light_t::Directional) ImGui::DragFloat3("Position", glm::value_ptr(m_light.position), 0.1f);
        if (m_light.type != light_t::Point) ImGui::DragFloat3("Direction", glm::value_ptr(m_light.direction), 0.1f);
        if (m_light.type == light_t::Spot) {
            ImGui::DragFloat("Inner Angle", &m_light.innerAngle, 1, 0, m_light.outerAngle);
            ImGui::DragFloat("Outer Angle", &m_light.outerAngle, 1, m_light.innerAngle, 90);
        }
        ImGui::DragFloat("Intensity", &m_light.intensity, 0.1f, 0, 10);
        if (m_light.type != light_t::Directional) ImGui::DragFloat("Range", &m_light.range, 0.1f, 0.1f, 50);
        ImGui::End();
        
        //m_angle += dt * 3;
        //m_transform.rotation.z += 30 * dt;
        // Model Movement
        m_transform.position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_A) ? m_speed * -dt : 0;
        m_transform.position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_D) ? m_speed *  dt : 0;
        m_transform.position.z += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_W) ? m_speed * -dt : 0;
        m_transform.position.z += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_S) ? m_speed *  dt : 0;
        // Model Rotation
        m_transform.rotation.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_UP) ? m_rotateSpeed * -dt : 0;
        m_transform.rotation.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_DOWN) ? m_rotateSpeed *  dt : 0;
        m_transform.rotation.y += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_LEFT) ? m_rotateSpeed * -dt : 0;
        m_transform.rotation.y += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_RIGHT) ? m_rotateSpeed *  dt : 0;
        m_transform.rotation.z += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_E) ? m_rotateSpeed * -dt : 0;
        m_transform.rotation.z += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_Q) ? m_rotateSpeed * dt : 0;
        if (ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_R)) {
            m_transform.position.x = 0.0f;
            m_transform.position.y = 0.0f;
            m_transform.position.z = 0.0f;
            m_transform.rotation.x = 0.0f;
            m_transform.rotation.y = 0.0f;
            m_transform.rotation.z = 0.0f;
            m_transform.scale.x = 1.0f;
            m_transform.scale.y = 1.0f;
            m_transform.scale.z = 1.0f;
        }
        m_time += dt;

        auto material = m_model->GetMaterial();
        material->ProcessGui();
        material->Bind();
        //for (int i = 0; i < 3; i++) {
        //    std::string name = "lights[" + std::to_string(i) + "]";
        //    material->GetProgram()->SetUniform(name + ".type", m_lights[i].type);
        //    material->GetProgram()->SetUniform(name + ".ambientLight", ambientLight);
        //    material->GetProgram()->SetUniform(name + ".diffuseLight", m_lights[i].color);
        //    material->GetProgram()->SetUniform(name + ".lightPosition", m_lights[i].position);
        //    material->GetProgram()->SetUniform(name + ".direction", glm::normalize(m_lights[i].direction));
        //    material->GetProgram()->SetUniform(name + ".innerAngle", glm::radians(m_lights[i].innerAngle));
        //    material->GetProgram()->SetUniform(name + ".outerAngle", glm::radians(m_lights[i].outerAngle));
        //    material->GetProgram()->SetUniform(name + ".intensity", m_lights[i].intensity);
        //    material->GetProgram()->SetUniform(name + ".range", m_lights[i].range);
        //}
        material->GetProgram()->SetUniform("light.type", m_light.type);
        material->GetProgram()->SetUniform("light.ambientLight", ambientLight); 
        material->GetProgram()->SetUniform("light.diffuseLight", m_light.color);
        material->GetProgram()->SetUniform("light.lightPosition", m_light.position);
        material->GetProgram()->SetUniform("light.direction", glm::normalize(m_light.direction));
        material->GetProgram()->SetUniform("light.innerAngle", glm::radians(m_light.innerAngle));
        material->GetProgram()->SetUniform("light.outerAngle", glm::radians(m_light.outerAngle));
        material->GetProgram()->SetUniform("light.intensity", m_light.intensity);
        material->GetProgram()->SetUniform("light.range", m_light.range);

        // model matrix
        material->GetProgram()->SetUniform("model", m_transform.GetMatrix());

        // view matrix
        glm::mat4 view = glm::lookAt(glm::vec3{ 0, 0, 3 }, glm::vec3{ 0, 0, 0 }, glm::vec3{ 0, 1, 0 });
        material->GetProgram()->SetUniform("view", view);

        // projection matrix
        glm::mat4 projection = glm::perspective(glm::radians(70.0f), ENGINE.GetSystem<Renderer>()->GetWidth() / (float)ENGINE.GetSystem<Renderer>()->GetHeight(), 0.01f, 100.0f);
        material->GetProgram()->SetUniform("projection", projection);
        ENGINE.GetSystem<Gui>()->EndFrame();
    }

    void World04::Draw(Renderer& renderer) {
        // pre-render
        renderer.BeginFrame();

        // render
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        m_model->Draw();

        ENGINE.GetSystem<Gui>()->Draw();
        // post-render
        renderer.EndFrame();
    }
}
