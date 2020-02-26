#pragma once

#include "../../include.h"

class RawFont;
class RawMesh;
class RawImage;
class GLObject;
class RawProgram;
class RawTexture;
class RawMaterial;
class Component;

namespace RenderPipline {
    struct RenderCommand {
        //  ����ö��
        enum EnabledEnum {
            kTargetColor0       = 0x1,      //  �����ɫ
            kTargetColor1       = 0x2,      //  �����ɫ

            //  ���ù���
            kClipView    = 0x4,         //  �����ü�
            kBlend       = 0x8,         //  �������
            kViewMat     = 0x10,        //  ʹ��VMat
            kProjMat     = 0x20,        //  ʹ��pMat
        };
        uint mEnabledFlag;

        std::function<void(const RenderCommand &, uint)> mCallback;
        void Call(uint pos) { if (mCallback)mCallback(*this, pos); }
        RenderCommand(uint enabledFlag): mEnabledFlag(enabledFlag){}
    };

    //  ������Ⱦ��
    struct FowardCommand : public RenderCommand {
        using PairImage = std::pair<std::string, SharePtr<RawImage>>;

        SharePtr<RawMesh>       mMesh;
        SharePtr<RawProgram>    mProgram;
        std::vector<PairImage>  mPairImages;
        glm::mat4               mTransform;

        //  ��ѡ����
        glm::vec4               mClipview;
        uint                    mBlendSrc;
        uint                    mBlendDst;
        uint                    mDrawMode;
        glm::mat4               mViewMat;
        glm::mat4               mProjMat;
        FowardCommand()
            : RenderCommand(0)
            , mTransform(0)
            , mClipview(0)
            , mBlendSrc(0)
            , mBlendDst(0)
            , mDrawMode(GL_TRIANGLES)
            , mViewMat(0)
            , mProjMat(0)
        { }
    };

    //  ������Ⱦ
    struct PostCommand : public RenderCommand {
        enum TypeEnum {
            kSample,        //  ����
            kSwap,          //  ����
        };
        TypeEnum                mType;
        SharePtr<RawMesh>       mMesh;
        SharePtr<RawProgram>    mProgram;
        glm::mat4               mTransform;

        //  ��ѡ����
        glm::vec4               mClipview;
        uint                    mBlendSrc;
        uint                    mBlendDst;
        glm::mat4               mViewMat;
        glm::mat4               mProjMat;
        PostCommand()
            : RenderCommand(0)
            , mTransform(0)
            , mClipview(0)
            , mBlendSrc(0)
            , mBlendDst(0)
            , mViewMat(0)
            , mProjMat(0)
            , mType(kSample)
        { }
    };

    //  �ֲ���Ⱦ
    struct TargetCommand : public RenderCommand {
        //  ��ջ/��ջ
        enum TypeEnum {
            kPush,
            kPop,
        };

        //  ���ò���
        TypeEnum                    mType;
        std::vector<PostCommand>    mPostCommands;      //  ����
        std::vector<FowardCommand>  mFowardCommands;    //  ����
        SharePtr<RawImage>          mRenderTextures[2]; //  ˫����

        //  ��ѡ����
        glm::vec4       mClearColor;
        glm::vec4       mClipView;

        TargetCommand()
            : RenderCommand(kTargetColor0 | kTargetColor1)
            , mType(kPush)
            , mClipView(0)
            , mClearColor(0)
        { }
    };

    struct MatrixStack {
    public:
        enum TypeEnum {
            kModel,
            kView,
            kProj,
            Length,
        };

        void Pop(TypeEnum mode)
        {
            GetStack(mode).pop();
        }

        void Push(TypeEnum mode)
        {
            Push(mode, GetStack(mode).top());
        }

        void Push(TypeEnum mode, const glm::mat4 & mat)
        {
            GetStack(mode).push(mat);
        }

        void Identity(TypeEnum mode)
        {
            GetStack(mode).push(glm::mat4(1));
        }

        void Identity(TypeEnum mode, const glm::mat4 & mat)
        {
            GetStack(mode).push(mat);
        }

        void Mul(TypeEnum mode, const glm::mat4 & mat)
        {
            Push(mode);
            GetStack(mode).top() *= mat;
        }

        const glm::mat4 & Top(TypeEnum mode)
        {
            return GetStack(mode).top();
        }

        const glm::mat4 & GetM()
        {
            return Top(TypeEnum::kModel);
        }

        const glm::mat4 & GetV()
        {
            return Top(TypeEnum::kView);
        }

        const glm::mat4 & GetP()
        {
            return Top(TypeEnum::kProj);
        }

    private:
        std::stack<glm::mat4> & GetStack(TypeEnum mode)
        {
            return _matrixs[(size_t)mode];
        }

    private:
        std::stack<glm::mat4> _matrixs[(uint)TypeEnum::Length];
    };
}