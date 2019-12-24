#include "ui_object.h"
#include "../../raw_sys/raw.h"
#include "../../raw_sys/raw_sys.h"
#include "../../raw_sys/comp_transform.h"
#include "../../editor_sys/editor_sys.h"

const glm::vec4 UIObjectGLCanvas::VAL_TrackPointColors[32] = {
    glm::vec4{ 0.60f, 0.85f, 0.91f, 1.0f },
    glm::vec4{ 0.60f, 0.85f, 0.91f, 1.0f },
    glm::vec4{ 0.60f, 0.85f, 0.91f, 1.0f },
    glm::vec4{ 0.60f, 0.85f, 0.91f, 1.0f },
};

UIObjectGLCanvas::UIObjectGLCanvas() : UIObject(UITypeEnum::kGLCanvas, new UIStateGLCanvas())
{
    auto state = GetState<UIStateGLCanvas>();
    state->mRenderTextures[0] = std::create_ptr<RawImage>();
    state->mRenderTextures[0]->InitNull(GL_RGBA);
    state->mRenderTextures[1] = std::create_ptr<RawImage>();
    state->mRenderTextures[1]->InitNull(GL_RGBA);
    state->mGLProgramSolidFill = Global::Ref().mRawSys->Get<RawProgram>(tools::GL_PROGRAM_SOLID_FILL);
}

void UIObjectGLCanvas::HandlePostCommands(RenderPipline::TargetCommand & command)
{
    auto state = GetState();
    std::swap(command.mRenderTextures[0]->mID,
              command.mRenderTextures[1]->mID);
    for (auto & cmd : command.mPostCommands)
    {
        for (auto i = 0; i != cmd.mProgram->GetPassCount(); ++i)
        {
            cmd.mProgram->UsePass(i);
            if (cmd.mEnabledFlag & RenderPipline::RenderCommand::kClipView)
            {
                glEnable(GL_SCISSOR_TEST);
                auto matrixVP = GetMatrixStack().GetP() * GetMatrixStack().GetV();
                auto min = ProjectScreen({ cmd.mClipview.x, cmd.mClipview.y });
                auto max = ProjectScreen({ cmd.mClipview.z, cmd.mClipview.w });
                min.y = state->Move.w - min.y;
                max.y = state->Move.w - max.y;
                if (min.x > max.x) { std::swap(min.x, max.x); }
                if (min.y > max.y) { std::swap(min.y, max.y); }
                glScissor(
                    (iint)min.x, (iint)min.y,
                    (iint)max.x - (iint)min.x,
                    (iint)max.y - (iint)min.y);
            }
            if (cmd.mEnabledFlag & RenderPipline::RenderCommand::kBlend)
            {
                glEnable(GL_BLEND);
                glBlendFunc(cmd.mBlendSrc, cmd.mBlendDst);
            }

            tools::RenderTargetAttachment(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, 
                                          GL_TEXTURE_2D, command.mRenderTextures[0]->mID);
            cmd.mProgram->BindUniformTex2D("uniform_screen", command.mRenderTextures[1]->mID, 0);
            cmd.Call(1);
            Post(
                cmd.mProgram,
                cmd.mTransform);
            cmd.mMesh->Draw(GL_TRIANGLES);
            //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            //cmd.mMesh->Draw(GL_TRIANGLES);
            //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            //cmd.mMesh->Draw(GL_POINTS);
            if (cmd.mType == RenderPipline::PostCommand::kSwap)
            {
                std::swap(command.mRenderTextures[0]->mID, command.mRenderTextures[1]->mID);
            }
            if (cmd.mEnabledFlag & RenderPipline::RenderCommand::kClipView)
            {
                glDisable(GL_SCISSOR_TEST);
            }
            if (cmd.mEnabledFlag & RenderPipline::RenderCommand::kBlend)
            {
                glDisable(GL_BLEND);
            }
        }
    }
}

void UIObjectGLCanvas::HandleFowardCommands(RenderPipline::TargetCommand & command)
{
    auto state = GetState();
    for (auto & cmd : command.mFowardCommands)
    {
        for (auto i = 0; i != cmd.mProgram->GetPassCount(); ++i)
        {
            uint texNum = 0;
            cmd.mProgram->UsePass(i);
            for (auto & pair : cmd.mPairImages)
            {
                cmd.mProgram->BindUniformTex2D(pair.first.c_str(), pair.second->mID, texNum++);
            }
            if (cmd.mEnabledFlag & RenderPipline::RenderCommand::kClipView)
            {
                glEnable(GL_SCISSOR_TEST);
                auto matrixVP = GetMatrixStack().GetP() * GetMatrixStack().GetV();
                auto min = ProjectScreen({ cmd.mClipview.x, cmd.mClipview.y });
                auto max = ProjectScreen({ cmd.mClipview.z, cmd.mClipview.w });
                min.y = state->Move.w - min.y;
                max.y = state->Move.w - max.y;
                if (min.x > max.x) { std::swap(min.x, max.x); }
                if (min.y > max.y) { std::swap(min.y, max.y); }
                glScissor(
                    (iint)min.x, (iint)min.y,
                    (iint)max.x - (iint)min.x,
                    (iint)max.y - (iint)min.y);
            }
            if (cmd.mEnabledFlag & RenderPipline::RenderCommand::kBlend)
            {
                glEnable(GL_BLEND);
                glBlendFunc(cmd.mBlendSrc, cmd.mBlendDst);
            }
            cmd.Call(texNum);
            Post(
                cmd.mProgram,
                cmd.mTransform);
            cmd.mMesh->Draw(GL_TRIANGLES);
            if (cmd.mEnabledFlag & RenderPipline::RenderCommand::kClipView)
            {
                glDisable(GL_SCISSOR_TEST);
            }
            if (cmd.mEnabledFlag & RenderPipline::RenderCommand::kBlend)
            {
                glDisable(GL_BLEND);
            }
        }
    }
}

void UIObjectGLCanvas::CollCommands()
{
    auto state = GetState<UIStateGLCanvas>();
    auto & command = state->mTargetCommandArray.emplace_back();
    command.mRenderTextures[0] = state->mRenderTextures[0];
    command.mRenderTextures[1] = state->mRenderTextures[1];
    Global::Ref().mEditorSys->GetProject()->GetObject()->Update(this, 0.0f);
}

void UIObjectGLCanvas::CallCommands()
{
    auto state = GetState<UIStateGLCanvas>();
    ASSERT_LOG(state->mTargetCommandStack == 0, "");
    tools::RenderTargetBind(state->mRenderTarget, GL_FRAMEBUFFER);

    //  ��ǰ�ӿ�, �ɵ��ӿ�, �µ��ӿ�
    glm::ivec4 viewport[3];
    glGetIntegerv(GL_VIEWPORT, &viewport[0][0]);
    glGetIntegerv(GL_VIEWPORT, &viewport[1][0]);

    for (auto iter = state->mTargetCommandArray.rbegin(); 
              iter != state->mTargetCommandArray.rend(); ++iter)
    {
        auto & command = *iter;

        if (command.mEnabledFlag & RenderPipline::TargetCommand::kUseCanvasSize)
        {
            viewport[2].x = 0; viewport[2].z = (int)state->Move.z;
            viewport[2].y = 0; viewport[2].w = (int)state->Move.w;
        }
        else
        {
            viewport[2] = command.mClipView;
        }

        //  �����ӿ�
        if (viewport[1] != viewport[2])
        {
            viewport[1] = viewport[2];
            glViewport(viewport[2].x, viewport[2].y, 
                       viewport[2].z, viewport[2].w);
        }

        if (command.mRenderTextures[0]->mW != viewport[2].z || command.mRenderTextures[0]->mH != viewport[2].w)
        {
            command.mRenderTextures[0]->ModifyWH(viewport[2].z, viewport[2].w);
        }

        if (command.mRenderTextures[1]->mW != viewport[2].z || command.mRenderTextures[1]->mH != viewport[2].w)
        {
            command.mRenderTextures[1]->ModifyWH(viewport[2].z, viewport[2].w);
        }

        tools::RenderTargetAttachment(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, command.mRenderTextures[0]->mID);
        tools::RenderTargetAttachment(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, command.mRenderTextures[1]->mID);
        glClearColor(command.mClearColor.x, command.mClearColor.y, command.mClearColor.z, command.mClearColor.w);
        uint buffers[2];
        if (command.mEnabledFlag & (RenderPipline::RenderCommand::kTargetColor0 | RenderPipline::RenderCommand::kTargetColor0))
        {
            buffers[0] = GL_COLOR_ATTACHMENT0;
            buffers[1] = GL_COLOR_ATTACHMENT1;
            glDrawBuffers(2, buffers);

        }
        else if (command.mEnabledFlag & RenderPipline::RenderCommand::kTargetColor0)
        {
            buffers[0] = GL_COLOR_ATTACHMENT0;
            buffers[1] = GL_NONE;
            glDrawBuffers(2, buffers);
        }
        else if (command.mEnabledFlag & RenderPipline::RenderCommand::kTargetColor1)
        {
            buffers[0] = GL_NONE;
            buffers[1] = GL_COLOR_ATTACHMENT1;
            glDrawBuffers(2, buffers);
        }
        glClear(GL_COLOR_BUFFER_BIT);
        buffers[0] = GL_COLOR_ATTACHMENT0;
        buffers[1] = GL_NONE;
        glDrawBuffers(2, buffers);

        if (!command.mFowardCommands.empty())
        {
            HandleFowardCommands(command);
        }
        if (!command.mPostCommands.empty())
        {
            HandlePostCommands(command);
        }
    }

    glViewport(0, 0, (int)state->Move.z, (int)state->Move.w);
    DrawTrackPoint();
    DrawSelectRect();

    glUseProgram(0);
    state->mTargetCommandArray.clear();
    glViewport(viewport[0].x, viewport[0].y, 
               viewport[0].z, viewport[0].w);
    tools::RenderTargetBind(0, GL_FRAMEBUFFER);

    ASSERT_LOG(glGetError() == 0, "");
}

void UIObjectGLCanvas::DrawTrackPoint()
{
    auto state = GetState<UIStateGLCanvas>();
    glLineWidth((float)VAL_TrackPointLine);
    glPointSize((float)VAL_TrackPointSize);
    for (auto & object : state->mOperation.mSelectObjects)
    {
        for (auto i0 = 0; i0 != object->GetComponents().size(); ++i0)
        {
            if (object->GetComponents().at(i0)->HasState(Component::StateEnum::kActive))
            {
                std::vector<RawMesh::Vertex> points;
                auto & component = object->GetComponents().at(i0);
                auto & trackPoints = component->GetTrackPoints();
                for (auto i1 = 0; i1 != trackPoints.size(); ++i1)
                {
                    if (state->mOperation.mEditComponent == component && state->mOperation.mEditTrackPoint == i1)
                    {
                        points.emplace_back(trackPoints.at(i1), VAL_TrackPointColors[i0] * 0.5f);
                    }
                    else
                    {
                        points.emplace_back(trackPoints.at(i1), VAL_TrackPointColors[i0]);
                    }
                }
                
                auto & mesh = GetMeshBuffer(i0);
                state->mGLProgramSolidFill->UsePass(0);
                Post(state->mGLProgramSolidFill,
                     object->GetWorldMatrix());
                mesh->Update(points, {});
                mesh->Draw(GL_LINE_LOOP);
                if (HasOpMode(UIStateGLCanvas::Operation::kEdit))
                {
                    mesh->Draw(GL_POINTS);
                }
            }
        }
    }
}

void UIObjectGLCanvas::DrawSelectRect()
{
    if (HasOpMode(UIStateGLCanvas::Operation::kSelect))
    {
        auto state = GetState<UIStateGLCanvas>();
        auto min = ProjectWorld(ToLocalCoord(glm::vec2(state->mOperation.mSelectRect.x, state->mOperation.mSelectRect.y)));
        auto max = ProjectWorld(ToLocalCoord(glm::vec2(state->mOperation.mSelectRect.z, state->mOperation.mSelectRect.w)));
        if (min.x > max.x) std::swap(min.x, max.x);
        if (min.y > max.y) std::swap(min.y, max.y);

        //  ���
        std::vector<RawMesh::Vertex> points;
        points.emplace_back(glm::vec2(min.x, min.y), glm::vec4(0.7f, 0.9f, 0.1f, 0.2f));
        points.emplace_back(glm::vec2(max.x, min.y), glm::vec4(0.7f, 0.9f, 0.1f, 0.2f));
        points.emplace_back(glm::vec2(max.x, max.y), glm::vec4(0.7f, 0.9f, 0.1f, 0.2f));
        points.emplace_back(glm::vec2(min.x, min.y), glm::vec4(0.7f, 0.9f, 0.1f, 0.2f));
        points.emplace_back(glm::vec2(max.x, max.y), glm::vec4(0.7f, 0.9f, 0.1f, 0.2f));
        points.emplace_back(glm::vec2(min.x, max.y), glm::vec4(0.7f, 0.9f, 0.1f, 0.2f));
        auto mesh0 = GetMeshBuffer(0);
        mesh0->Update(points, {});

        //  ���
        points.clear();
        points.emplace_back(glm::vec2(min.x, min.y), glm::vec4(0.7f, 0.9f, 0.1f, 0.5f));
        points.emplace_back(glm::vec2(max.x, min.y), glm::vec4(0.7f, 0.9f, 0.1f, 0.5f));
        points.emplace_back(glm::vec2(max.x, max.y), glm::vec4(0.7f, 0.9f, 0.1f, 0.5f));
        points.emplace_back(glm::vec2(min.x, max.y), glm::vec4(0.7f, 0.9f, 0.1f, 0.5f));
        auto mesh1 = GetMeshBuffer(1);
        mesh1->Update(points, {});

        state->mGLProgramSolidFill->UsePass(0);
        glEnable(GL_BLEND);
        Post(state->mGLProgramSolidFill, glm::mat4(1));
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        mesh0->Draw(GL_TRIANGLES);
        mesh1->Draw(GL_LINE_LOOP);
        glDisable(GL_BLEND);

        FromRectSelectObjects(min, max);
    }
}

void UIObjectGLCanvas::Post(const RenderPipline::PostCommand & cmd)
{
    auto state = GetState<UIStateGLCanvas>();
    ASSERT_LOG(state->mTargetCommandStack < state->mTargetCommandArray.size(), "");
    state->mTargetCommandArray.at(state->mTargetCommandStack).mPostCommands.emplace_back(cmd);
}

void UIObjectGLCanvas::Post(const::RenderPipline::TargetCommand & cmd)
{
    auto state = GetState<UIStateGLCanvas>();
    if      (cmd.mType == RenderPipline::TargetCommand::TypeEnum::kPush)
    {
        auto & command = state->mTargetCommandArray.emplace_back(cmd);
        //command.mRenderTextures[0] = command.mRenderTextures[0];
        command.mRenderTextures[1] = state->mRenderTextures[1];
        ++state->mTargetCommandStack;
    }
    else if (cmd.mType == RenderPipline::TargetCommand::TypeEnum::kPop)
    {
        ASSERT_LOG(state->mTargetCommandStack != 0, "");
        --state->mTargetCommandStack;
    }
}

void UIObjectGLCanvas::Post(const RenderPipline::FowardCommand & cmd)
{
    auto state = GetState<UIStateGLCanvas>();
    ASSERT_LOG(state->mTargetCommandStack < state->mTargetCommandArray.size(), "");
    state->mTargetCommandArray.at(state->mTargetCommandStack).mFowardCommands.emplace_back(cmd);
}

void UIObjectGLCanvas::Post(const SharePtr<RawProgram> & program, const glm::mat4 & transform)
{
    auto state = GetState<UIStateGLCanvas>();
    const auto & matrixV = GetMatrixStack().Top(RenderPipline::MatrixStack::TypeEnum::kView);
    const auto & matrixP = GetMatrixStack().Top(RenderPipline::MatrixStack::TypeEnum::kProj);
    const auto & matrixMV = matrixV * transform;
    const auto & matrixVP = matrixP * matrixV;
    const auto & matrixMVP = matrixP * matrixMV;

    //  ����
    program->BindUniformMatrix("uniform_matrix_m", transform);
    program->BindUniformMatrix("uniform_matrix_v", matrixV);
    program->BindUniformMatrix("uniform_matrix_p", matrixP);
    program->BindUniformMatrix("uniform_matrix_mv", matrixMV);
    program->BindUniformMatrix("uniform_matrix_vp", matrixVP);
    program->BindUniformMatrix("uniform_matrix_mvp", matrixMVP);

    //  �����
    program->BindUniformMatrix("uniform_matrix_v_inv", glm::inverse(matrixV));
    program->BindUniformMatrix("uniform_matrix_p_inv", glm::inverse(matrixP));
    program->BindUniformMatrix("uniform_matrix_mv_inv", glm::inverse(matrixMV));
    program->BindUniformMatrix("uniform_matrix_vp_inv", glm::inverse(matrixVP));

    //  ��������
    program->BindUniformNumber("uniform_game_time", glfwGetTime());
}

void UIObjectGLCanvas::OpEditObject(const SharePtr<GLObject> & object)
{
    auto state = GetState<UIStateGLCanvas>();
    if (object != nullptr)
    {
        AddOpMode(UIStateGLCanvas::Operation::kEdit, true);
        state->mOperation.mEditObject = object;
    }
    else
    {
        AddOpMode(UIStateGLCanvas::Operation::kEdit, false);
        state->mOperation.mEditObject       = nullptr;
        state->mOperation.mEditComponent    = nullptr;
        state->mOperation.mEditTrackPoint   = std::numeric_limits<uint>::max();
    }
}

void UIObjectGLCanvas::OpDragSelects(const glm::vec2 & worldBeg, const glm::vec2 & worldEnd)
{ 
    static const auto IsSkip = [] (
        const SharePtr<GLObject>              & object, 
        const std::vector<SharePtr<GLObject>> & objects)
    {
        for (auto parent = object->GetParent(); parent != nullptr; parent = parent->GetParent())
        {
            if (std::find(objects.begin(), objects.end(), parent) != objects.end())
            {
                return true;
            }
        }
        return false;
    };

    auto state = GetState<UIStateGLCanvas>();
    for (auto & object : state->mOperation.mSelectObjects)
    {
        if (!IsSkip(object, state->mOperation.mSelectObjects))
        {
            auto a = object->GetParent()->WorldToLocal(worldBeg);
            auto b = object->GetParent()->WorldToLocal(worldEnd);
            auto ab = b - a;
            object->GetTransform()->AddPosition(ab.x, ab.y);
        }
    }
}

void UIObjectGLCanvas::OpSelected(const SharePtr<GLObject> & object, bool selected)
{
    auto state = GetState<UIStateGLCanvas>();
    auto it = std::find(state->mOperation.mSelectObjects.begin(),
                        state->mOperation.mSelectObjects.end(), object);
    if (selected)
    {
        if (it == state->mOperation.mSelectObjects.end())
        {
            state->mOperation.mSelectObjects.push_back(object);
        }
    }
    else
    {
        if (it != state->mOperation.mSelectObjects.end())
        {
            state->mOperation.mSelectObjects.erase(it);
        }
    }
}

void UIObjectGLCanvas::OpSelectedClear()
{
    auto state = GetState<UIStateGLCanvas>();
    state->mOperation.mSelectObjects.clear();

    state->mOperation.mOpMode = 0;
    state->mOperation.mEditObject = nullptr;
    state->mOperation.mEditComponent = nullptr;
}

glm::vec2 UIObjectGLCanvas::ProjectScreen(const glm::vec2 & world)
{
    glm::vec4 viewPort(0, 0, GetState()->Move.z, GetState()->Move.w);
    auto coord = glm::project(glm::vec3(world, 0),
            GetMatView(), GetMatProj(), viewPort);
    return  glm::vec2(coord.x, GetState()->Move.w - coord.y);
}

glm::vec2 UIObjectGLCanvas::ProjectWorld(const glm::vec2 & screen)
{
    glm::vec3 coord(screen.x, GetState()->Move.w - screen.y, 0);
    glm::vec4 viewPort(0, 0, GetState()->Move.z, GetState()->Move.w);
    return glm::unProject(coord, GetMatView(), GetMatProj(), viewPort);
}

RenderPipline::MatrixStack & UIObjectGLCanvas::GetMatrixStack()
{
    return GetState<UIStateGLCanvas>()->mMatrixStack;
}

bool UIObjectGLCanvas::OnEnter()
{
    if (Global::Ref().mEditorSys->IsOpenProject())
    {
        auto state = GetState<UIStateGLCanvas>();
        state->mMatrixStack.Identity(RenderPipline::MatrixStack::TypeEnum::kModel);
        state->mMatrixStack.Identity(RenderPipline::MatrixStack::TypeEnum::kView, GetMatView());
        state->mMatrixStack.Identity(RenderPipline::MatrixStack::TypeEnum::kProj, GetMatProj());
        CollCommands();
        CallCommands();
        state->mMatrixStack.Pop(RenderPipline::MatrixStack::TypeEnum::kModel);
        state->mMatrixStack.Pop(RenderPipline::MatrixStack::TypeEnum::kView);
        state->mMatrixStack.Pop(RenderPipline::MatrixStack::TypeEnum::kProj);
        return true;
    }
    return false;
}

void UIObjectGLCanvas::OnLeave(bool ret)
{
    if (ret)
    {
        auto state = GetState<UIStateGLCanvas>();
        ImGui::Image((ImTextureID)state->mRenderTextures[0]->mID, 
                      ImVec2(state->Move.z, state->Move.w),
                      ImVec2(0, 1), ImVec2(1, 0));
    }
}

bool UIObjectGLCanvas::OnCallEventMessage(UIEventEnum e, const UIEvent::Event & param)
{
    if (Global::Ref().mEditorSys->IsOpenProject())
    {
        switch (e)
        {
        case UIEventEnum::kKey:
            return OnEventKey((const UIEvent::Key &)param);
        case UIEventEnum::kMenu:
            return OnEventMenu((const UIEvent::Menu &)param);
        case UIEventEnum::kMouse:
            return OnEventMouse((const UIEvent::Mouse &)param);
        }
    }
    return false;
}

bool UIObjectGLCanvas::OnEventKey(const UIEvent::Key & param)
{
    auto ready = false;
    auto state = GetState<UIStateGLCanvas>();
    if (param.mState == 0 && param.mAct == 2 && param.mKey == GLFW_KEY_ENTER)
    {
        if (HasOpMode(UIStateGLCanvas::Operation::kEdit))
        {
            OpEditObject(nullptr);
        }
        else if (state->mOperation.mSelectObjects.size() == 1)
        {
            OpEditObject(state->mOperation.mSelectObjects.front());
        }
        ready = true;
    }
    return ready;
}

bool UIObjectGLCanvas::OnEventMenu(const UIEvent::Menu & param)
{
    return false;
}

bool UIObjectGLCanvas::OnEventMouse(const UIEvent::Mouse & param)
{
    auto state = GetState<UIStateGLCanvas>();
    //  �����м��϶���̨
    if (param.mAct == 1 && param.mKey == 2)
    {
        state->mOperation.mViewCoord.x -= param.mDelta.x;
        state->mOperation.mViewCoord.y += param.mDelta.y;
    }
    //  �������������̨
    if (param.mWheel != 0)
    {
        auto origin = ProjectWorld(ToLocalCoord(param.mMouse));
        auto oldS   = state->mOperation.mViewScale;
        state->mOperation.mViewScale = std::clamp(state->mOperation.mViewScale + (0.2f * param.mWheel), 0.2f, 10.0f);
        auto newS   = state->mOperation.mViewScale;
        auto target = ProjectWorld(ToLocalCoord(param.mMouse));
        state->mOperation.mViewCoord.x += (origin.x - target.x) * newS;
        state->mOperation.mViewCoord.y += (origin.y - target.y) * newS;
    }

    //  �Ǳ༭ģʽ
    if (!HasOpMode(UIStateGLCanvas::Operation::kEdit))
    {
        //  �������ѡ�ж���
        if (!HasOpMode(UIStateGLCanvas::Operation::kSelect) && param.mAct == 3 && param.mKey == 0)
        {
            state->mOperation.mSelectRect.x = param.mMouse.x;
            state->mOperation.mSelectRect.y = param.mMouse.y;
            state->mOperation.mSelectRect.z = param.mMouse.x;
            state->mOperation.mSelectRect.w = param.mMouse.y;
            const auto & coord = ProjectWorld(ToLocalCoord(param.mMouse));
            if (FromRectSelectObjects(coord, coord))
            {
                AddOpMode(UIStateGLCanvas::Operation::kDrag, true);
            }
            else
            {
                AddOpMode(UIStateGLCanvas::Operation::kSelect, true);
            }
        }
        //  �������ѡ�����
        if (HasOpMode(UIStateGLCanvas::Operation::kSelect) && param.mAct == 1 && param.mKey == 0)
        {
            state->mOperation.mSelectRect.z = param.mMouse.x;
            state->mOperation.mSelectRect.w = param.mMouse.y;
        }
        //  ̧���������ѡ��
        if (HasOpMode(UIStateGLCanvas::Operation::kSelect) && param.mAct == 2 && param.mKey == 0)
        {
            AddOpMode(UIStateGLCanvas::Operation::kSelect, false);
        }
        //   ��ס����϶�����
        if (HasOpMode(UIStateGLCanvas::Operation::kDrag) && param.mAct == 1 && param.mKey == 0)
        {
            auto prev = ProjectWorld(ToLocalCoord(param.mMouse - param.mDelta));
            auto curr = ProjectWorld(ToLocalCoord(param.mMouse               ));
            OpDragSelects(prev, curr);
        }
        //  ̧�����������ק
        if (HasOpMode(UIStateGLCanvas::Operation::kDrag) && param.mAct == 2 && param.mKey == 0)
        {
            AddOpMode(UIStateGLCanvas::Operation::kDrag, false);
        }
    }

    //  �༭ģʽ
    if (HasOpMode(UIStateGLCanvas::Operation::kEdit))
    {
        ASSERT_LOG(state->mOperation.mEditObject != nullptr, "");
        //  ���˫����������
        if (param.mAct == 4 && param.mKey == 0)
        {
            auto[num, comp, coord, index] = FromCoordSelectTrackPoint(ToLocalCoord(param.mMouse));
            if (num == 0 || num == 2)
            {
                comp->InsertTrackPoint(index, coord);
            }
        }
        //  �������ѡ�ж���
        if (param.mAct == 3 && param.mKey == 0)
        {
            auto[num, comp, coord, index] = FromCoordSelectTrackPoint(ToLocalCoord(param.mMouse));
            state->mOperation.mEditTrackPoint = num == 1? index: (uint)-1;
            state->mOperation.mEditComponent = num == 1? comp: nullptr;
        }
        //  ��������϶�����
        if (param.mAct == 1 && param.mKey == 0 && state->mOperation.mEditComponent)
        {
            auto & comp = state->mOperation.mEditComponent;
            auto index = state->mOperation.mEditTrackPoint;
            ASSERT_LOG(index < comp->GetTrackPoints().size(), "");
            auto prev = comp->GetOwner()->WorldToLocal(ProjectWorld(ToLocalCoord(param.mMouse - param.mDelta)));
            auto curr = comp->GetOwner()->WorldToLocal(ProjectWorld(ToLocalCoord(param.mMouse               )));
            comp->ModifyTrackPoint(index, comp->GetTrackPoints().at(index) + (curr - prev));
        }
    }
    return true;
}

glm::mat4 UIObjectGLCanvas::GetMatView()
{
    auto state = GetState<UIStateGLCanvas>();
    auto view = glm::lookAt(state->mOperation.mViewCoord,
                            state->mOperation.mViewCoord - glm::vec3(0, 0, 1),glm::vec3(0, 1, 0));
    return glm::scale(view, glm::vec3(state->mOperation.mViewScale, state->mOperation.mViewScale, 1));
}

glm::mat4 UIObjectGLCanvas::GetMatProj()
{
    auto state = GetState<UIStateGLCanvas>();
    return glm::ortho(state->Move.z * -0.5f, state->Move.z * 0.5f, 
                      state->Move.w * -0.5f, state->Move.w * 0.5f);
}

glm::mat4 UIObjectGLCanvas::GetMatViewProj()
{
    return GetMatProj() * GetMatView();
}

SharePtr<RawMesh> & UIObjectGLCanvas::GetMeshBuffer(size_t idx)
{
    auto state = GetState<UIStateGLCanvas>();
    if (idx == state->mMeshBuffer.size())
    {
        auto mesh = std::create_ptr<RawMesh>();
        mesh->Init({}, {}, RawMesh::Vertex::kV 
                         | RawMesh::Vertex::kC);
        state->mMeshBuffer.push_back(mesh);
    }
    return state->mMeshBuffer.at(idx);
}

bool UIObjectGLCanvas::HasOpMode(UIStateGLCanvas::Operation::OpModeEnum op)
{
    return op & GetState<UIStateGLCanvas>()->mOperation.mOpMode;
}

void UIObjectGLCanvas::AddOpMode(UIStateGLCanvas::Operation::OpModeEnum op, bool add)
{
    auto state = GetState<UIStateGLCanvas>();
    if (add) state->mOperation.mOpMode |=  op;
    else     state->mOperation.mOpMode &= ~op;
}

const SharePtr<GLObject>& UIObjectGLCanvas::GetProjectRoot()
{
    ASSERT_LOG(Global::Ref().mEditorSys->IsOpenProject(), "");
    return Global::Ref().mEditorSys->GetProject()->GetObject();
}

bool UIObjectGLCanvas::FromRectSelectObjects(const glm::vec2 & worldMin, const glm::vec2 & worldMax)
{
    auto state = GetState<UIStateGLCanvas>();
    if (worldMin == worldMax)
    {
        if (auto hit = FromCoordSelectObject(GetProjectRoot(), GetProjectRoot()->ParentToLocal(worldMin)))
        {
            auto ret = std::find(state->mOperation.mSelectObjects.begin(), 
                                 state->mOperation.mSelectObjects.end(), hit);
            if (ret == state->mOperation.mSelectObjects.end())
            {
                Global::Ref().mEditorSys->OptSelectObject(hit, true);
            }
            return true;    //  ������һ��Object
        }
        else
        {
            Global::Ref().mEditorSys->OptSelectObject(hit, true);
        }
    }
    else
    {
        std::vector<SharePtr<GLObject>> output;
        auto pt0 = GetProjectRoot()->ParentToLocal(worldMin);
        auto pt1 = GetProjectRoot()->ParentToLocal(glm::vec2(worldMax.x, worldMin.y));
        auto pt2 = GetProjectRoot()->ParentToLocal(worldMax);
        auto pt3 = GetProjectRoot()->ParentToLocal(glm::vec2(worldMin.x, worldMax.y));
        FromRectSelectObjects(GetProjectRoot(), pt0, pt1, pt2, pt3, output);
        auto NoExists = [&output] (const auto & object) 
        {
            return output.end() == std::find(output.begin(), output.end(), object);
        };
        std::vector<SharePtr<GLObject>> unlocks;
        std::copy_if(state->mOperation.mSelectObjects.begin(),
                     state->mOperation.mSelectObjects.end(),
                     std::back_inserter(unlocks), NoExists);
        for (auto & object : unlocks)
        {
            Global::Ref().mEditorSys->OptSelectObject(object, false);
        }
        for (auto & object : output)
        {
            Global::Ref().mEditorSys->OptSelectObject(object, true, true);
        }
    }
    return false;
}

void UIObjectGLCanvas::FromRectSelectObjects(
    const SharePtr<GLObject> & object, 
    const glm::vec2 & local0, 
    const glm::vec2 & local1, 
    const glm::vec2 & local2, 
    const glm::vec2 & local3, 
    std::vector<SharePtr<GLObject>> & output)
{
    std::vector<glm::vec2> points(4);
    auto pred = [&points] (const auto & com)
    { 
        auto it = std::find_if(com->GetTrackPoints().begin(), com->GetTrackPoints().end(),
            [&] (const auto & point) { return tools::IsContainsConvex(points, point); });
        return com->HasState(Component::StateEnum::kActive)
            && it != com->GetTrackPoints().end();
    };

    for (auto & children : object->GetObjects())
    {
        if (children->HasState(GLObject::StateEnum::kActive))
        {
            points.at(0) = children->ParentToLocal(local0);
            points.at(1) = children->ParentToLocal(local1);
            points.at(2) = children->ParentToLocal(local2);
            points.at(3) = children->ParentToLocal(local3);
            if (!children->HasState(GLObject::StateEnum::kLocked))
            {
                auto ret = std::find_if(
                    children->GetComponents().begin(),
                    children->GetComponents().end(), pred);
                if (children->GetComponents().end() != ret)
                {
                    output.push_back(children);
                }
            }
            FromRectSelectObjects(children, points.at(0), points.at(1), points.at(2), points.at(3), output);
        }
    }
}

SharePtr<GLObject> UIObjectGLCanvas::FromCoordSelectObject(const SharePtr<GLObject> & object, const glm::vec2 & local)
{
    auto thit = local;
    auto pred = [&thit] (const auto & com)
    { 
        return com->HasState(Component::StateEnum::kActive) &&
               tools::IsContains(com->GetTrackPoints(), thit);
    };
    for (auto it = object->GetObjects().rbegin(); it != object->GetObjects().rend(); ++it)
    {
        if ((*it)->HasState(GLObject::StateEnum::kActive))
        {
            thit = (*it)->ParentToLocal(local);

            if (auto ret = FromCoordSelectObject(*it, thit))
            {
                return ret;
            }
            if (!(*it)->HasState(GLObject::StateEnum::kLocked))
            {
                auto ret = std::find_if(
                    (*it)->GetComponents().begin(), 
                    (*it)->GetComponents().end(), pred);
                if (ret != (*it)->GetComponents().end()) { return *it; }
            }
        }
    }
    return nullptr;
}

std::tuple<iint, SharePtr<Component>, glm::vec2, uint> UIObjectGLCanvas::FromCoordSelectTrackPoint(const glm::vec2 & screen)
{
    //  ����ֵ:
    //      û�л���׷�ٵ� -1
    //      ����׷�ٵ�����  1
    //      ����׷�ٵ�֮��  2
    //      ����׷�ٵ㸽��  0
    auto state = GetState<UIStateGLCanvas>();
    ASSERT_LOG(state->mOperation.mEditObject != nullptr, "");

    const auto & comps = state->mOperation.mEditObject->GetComponents();
    for (auto it = comps.rbegin(); it != comps.rend(); ++it)
    {
        //  �����ж��Ƿ���ж���
        for (auto i = 0u; i != (*it)->GetTrackPoints().size(); ++i)
        {
            const auto coord = ProjectScreen(state->mOperation.mEditObject->LocalToWorld((*it)->GetTrackPoints().at(i)));

            if (tools::IsCantains(coord, (float)VAL_TrackPointSize, screen))
            {
                return std::make_tuple(1, *it, state->mOperation.mEditObject->WorldToLocal(ProjectWorld(screen)), (uint)i);
            }
            if (2 * VAL_TrackPointSize * VAL_TrackPointSize >= glm::length_sqrt(screen - coord))
            {
                return std::make_tuple(0, *it, state->mOperation.mEditObject->WorldToLocal(ProjectWorld(screen)), (uint)i + 1);
            }
        }
        //  ����ж��Ƿ�����߶�
        for (auto i = 0u; i != (*it)->GetTrackPoints().size(); ++i)
        {
            auto j = (i + 1) % (*it)->GetTrackPoints().size();
            const auto a = ProjectScreen(state->mOperation.mEditObject->LocalToWorld((*it)->GetTrackPoints().at(i)));
            const auto b = ProjectScreen(state->mOperation.mEditObject->LocalToWorld((*it)->GetTrackPoints().at(j)));
            if (VAL_TrackPointSize * VAL_TrackPointSize >= glm::length_sqrt(tools::PointToSegment(screen, a, b)))
            {
                return std::make_tuple(2, *it, state->mOperation.mEditObject->WorldToLocal(ProjectWorld(screen)), (uint)j);
            }
        }
    }
    return std::make_tuple(-1, nullptr, glm::vec2(), (uint)-1);
}
