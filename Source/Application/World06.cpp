#include "World06.h"
#include "Framework/Framework.h"
#include "Input/InputSystem.h"
#include <glm/glm/gtc/type_ptr.hpp>
#include <glm/glm/gtx/color_space.hpp>


namespace nc {
    bool World06::Initialize() {
        m_scene = std::make_unique<Scene>();
        m_scene->Load("scenes/scene_framebuffer.json");
        m_scene->Initialize();

        auto texture = std::make_shared<Texture>();
        texture->CreateTexture(512, 512);
        ADD_RESOURCE("fb_texture", texture);

        auto framebuffer = std::make_shared<Framebuffer>();
        framebuffer->CreateFramebuffer(texture);
        ADD_RESOURCE("fb", framebuffer);

        auto material = GET_RESOURCE(Material, "materials/framebuffer.mtrl");
        if (material) {
            material->albedoTexture = texture;
        }

        for (int i = 0; i < 2; i++) {
            auto actor = CREATE_CLASS_BASE(Actor, "tree");
            actor->name = CreateUnique("tree");
            actor->transform.position = glm::vec3{ randomf(-10, 10), 0, randomf(-10, 10) };
            actor->transform.scale = glm::vec3{ randomf(0.5f, 3.0f), randomf(0.5f, 3.0f), 0 };
            actor->Initialize();
            m_scene->Add(std::move(actor));
        }
        return true;
    }

    void World06::Shutdown() {
    }

    void World06::Update(float dt) {
        m_time += dt;
        ENGINE.GetSystem<Gui>()->BeginFrame();
        m_scene->Update(dt);
        m_scene->ProcessGui();
        
        //m_angle += dt * 3;
        //m_transform.rotation.z += 30 * dt;
        // Model Movement
        auto actor = m_scene->GetActorByName<Actor>("actor1");
        /*actor->transform.position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_A) ? m_speed * -dt : 0;
        actor->transform.position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_D) ? m_speed *  dt : 0;
        actor->transform.position.z += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_W) ? m_speed * -dt : 0;
        actor->transform.position.z += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_S) ? m_speed *  dt : 0;
        // Model Rotation
        actor->transform.rotation.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_UP) ? m_rotateSpeed * -dt : 0;
        actor->transform.rotation.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_DOWN) ? m_rotateSpeed *  dt : 0;
        actor->transform.rotation.y += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_LEFT) ? m_rotateSpeed * -dt : 0;
        actor->transform.rotation.y += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_RIGHT) ? m_rotateSpeed *  dt : 0;
        actor->transform.rotation.z += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_E) ? m_rotateSpeed * -dt : 0;
        actor->transform.rotation.z += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_Q) ? m_rotateSpeed * dt : 0;
        if (ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_R)) {
            actor->transform.position.x = 0.0f;
            actor->transform.position.y = 0.0f;
            actor->transform.position.z = 0.0f;
            actor->transform.rotation.x = 0.0f;
            actor->transform.rotation.y = 0.0f;
            actor->transform.rotation.z = 0.0f;
            actor->transform.scale.x = 1.0f;
            actor->transform.scale.y = 1.0f;
            actor->transform.scale.z = 1.0f;
        } */

        auto material = actor->GetComponent<ModelComponent>()->m_material;
        material->ProcessGui();
        material->Bind();

        material = GET_RESOURCE(Material, "Materials/refraction.mtrl");
        if (material) {
            ImGui::Begin("Refraction");
            ImGui::DragFloat("IOR", &m_refraction, 0.01f, 1, 3);
            auto program = material->GetProgram();
            program->Use();
            program->SetUniform("ior", m_refraction);
            ImGui::End();
        }

        
        //material->GetProgram()->SetUniform("light.ambientLight", m_ambientLight); 
        

        // model matrix
        //material->GetProgram()->SetUniform("model", m_transform.GetMatrix());

        //// view matrix
        //glm::mat4 view = glm::lookAt(glm::vec3{ 0, 0, 3 }, glm::vec3{ 0, 0, 0 }, glm::vec3{ 0, 1, 0 });
        //material->GetProgram()->SetUniform("view", view);

        //// projection matrix
        //glm::mat4 projection = glm::perspective(glm::radians(70.0f), ENGINE.GetSystem<Renderer>()->GetWidth() / (float)ENGINE.GetSystem<Renderer>()->GetHeight(), 0.01f, 100.0f);
        //material->GetProgram()->SetUniform("projection", projection);
        //ENGINE.GetSystem<Gui>()->EndFrame();
    }

    void World06::Draw(Renderer& renderer) {
        // *** PASS 1 ***
        m_scene->GetActorByName("cube")->active = false;

        auto framebuffer = GET_RESOURCE(Framebuffer, "fb");
        renderer.SetViewport(framebuffer->GetSize().x, framebuffer->GetSize().y);
        framebuffer->Bind();

        renderer.BeginFrame(glm::vec3{ 0, 0, 1 });
        m_scene->Draw(renderer);

        framebuffer->Unbind();

        // *** PASS 2 ***
        m_scene->GetActorByName("cube")->active = true;

        renderer.ResetViewport();
        renderer.BeginFrame();
        m_scene->Draw(renderer);

        ENGINE.GetSystem<Gui>()->Draw();

        renderer.EndFrame();
    }
}
