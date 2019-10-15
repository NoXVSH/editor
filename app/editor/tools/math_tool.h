#pragma once

namespace tools {
    //  �Ƿ������
    inline bool IsContain(const glm::vec4 & rect, const glm::vec2 & point)
    {
        return rect.x <= point.x
            && rect.y <= point.y
            && rect.x + rect.z >= point.x
            && rect.y + rect.w >= point.y;
    }

    //  �Ƿ��ڱ���
    inline int IsOnRect(const glm::vec4 & rect, const glm::vec2 & point, float border = 0)
    {
        auto t = rect.y, b = rect.y + rect.w;
        auto l = rect.x, r = rect.x + rect.z;
        if (point.x >= l && point.x <= r && std::abs(point.y - rect.y) <= border) return 0;
        if (point.x >= l && point.x <= r && std::abs(point.y - (rect.y + rect.w)) <= border) return 1;
        if (point.y >= t && point.y <= b && std::abs(point.x - rect.x) <= border) return 2;
        if (point.y >= t && point.y <= b && std::abs(point.x - (rect.x + rect.z)) <= border) return 3;
        return -1;
    }

    //  �㵽�ı��ξ���
    //      ���ؾ��뼰��ӽ��ı�
    inline std::pair<float, int> RectInDistance(const glm::vec4 & rect, const glm::vec2 & point)
    {
        auto t = std::abs(point.y - rect.y);
        auto b = std::abs(point.y - rect.y - rect.w);
        auto l = std::abs(point.x - rect.x);
        auto r = std::abs(point.x - rect.x - rect.z);
        //  ����ˮƽ��Χ��
        if (point.x >= rect.x && point.x <= rect.x + rect.z) { l = r = 0; }
        //  ���δ�ֱ��Χ��
        if (point.y >= rect.y && point.y <= rect.y + rect.w) { t = b = 0; }

        auto xmin = std::min(l, r);
        auto ymin = std::min(t, b);
        //  ������
        if (xmin == 0 && ymin == 0) { return std::make_pair(0.0f, 0); }
        //  ��ֱ����
        if (xmin == 0) { return std::make_pair(ymin, ymin == t ? 0 : 1); }
        //  ˮƽ����
        if (ymin == 0) { return std::make_pair(xmin, xmin == l ? 2 : 3); }
        //  ����Զ�ľ���
        auto dist = std::max(xmin, ymin);
        return std::make_pair(dist, dist == t ? 0
                                  : dist == b ? 1
                                  : dist == l ? 2 : 3);
    }

    //  �������Ƚ�
    inline bool Equal(const float f0, const float f1)
    {
        return std::abs(f1 - f0) <= FLT_EPSILON;
    }

    //  ���Ƿ����߶���
    inline bool IsOnSegment(const glm::vec2 & p, const glm::vec2 & a, const glm::vec2 & b)
    {
        return p.x >= std::min(a.x, b.x)
            && p.x <= std::max(a.x, b.x)
            && p.y >= std::min(a.y, b.y)
            && p.y <= std::max(a.y, b.y)
            && glm::cross(glm::vec3(p - a, 0), glm::vec3(b - p, 0)).z == 0.0f;
    }

    //  ���Ƿ����߶���
    inline bool IsOnSegment(const glm::vec2 & p, const std::vector<glm::vec2> & points)
    {
        auto size = points.size();
        for (auto i = 0; i != size; ++i)
        {
            auto & a = points.at(i             );
            auto & b = points.at((i + 1) % size);
            if (IsOnSegment(p, a, b)) { return true; }
        }
        return false;
    }

    //  ֱ�߽���
    inline bool IsCrossLine(const glm::vec2 & a, const glm::vec2 & b, const glm::vec2 & c, const glm::vec2 & d, float * crossA, float * crossB)
    {
        assert(crossA != nullptr);
        assert(crossB != nullptr);
        auto cross = glm::cross(glm::vec3(b - a, 0), glm::vec3(d - c, 0)).z;
        if (cross != 0.0)
        {
            *crossA = glm::cross(glm::vec3(d - c, 0), glm::vec3(a - c, 0)).z / cross;
            *crossB = glm::cross(glm::vec3(b - a, 0), glm::vec3(a - c, 0)).z / cross;
            return true;
        }
        return false;
    }

    //  �߶��ཻ
    inline bool IsCrossSegment(const glm::vec2 & a, const glm::vec2 & b, const glm::vec2 & c, const glm::vec2 & d)
    {
        auto ab = glm::vec3(b - a, 0);
        auto cd = glm::vec3(d - c, 0);
        return glm::cross(ab, glm::vec3(c - a, 0)).z * glm::cross(ab, glm::vec3(d - a, 0)).z <= 0
            && glm::cross(cd, glm::vec3(a - c, 0)).z * glm::cross(cd, glm::vec3(b - c, 0)).z <= 0;
    }

    //  �߶ν���
    inline bool IsCrossSegment(const glm::vec2 & a, const glm::vec2 & b, const glm::vec2 & c, const glm::vec2 & d, float * crossA, float * crossB)
    {
        if (IsCrossLine(a, b, c, d, crossA, crossB))
        {
            return *crossA >= 0.0f && *crossA <= 1.0f
                && *crossB >= 0.0f && *crossB <= 1.0f;
        }
        return false;
    }

    //  ���Ƿ��ڶ������
    inline bool IsInPolygon(const glm::vec2 & a, const std::vector<glm::vec2> & points)
    {
        glm::vec2 b(std::numeric_limits<float>::max(), a.y);

        auto num  = 0;
        auto size = points.size();
        for (auto i = 0; i != size; ++i)
        {
            auto & c = points.at(i             );
            auto & d = points.at((i + 1) % size);
            if (IsOnSegment(a, c, d)) { return true; }
            if (c.y != d.y)
            {
                if (IsCrossSegment(a, b, c, d))
                {
                    if (c.y < d.y)
                    { if (c.y < a.y) ++num; }
                    else
                    { if (d.y < a.y) ++num; }
                }
            }
        }
        return (num & 1) == 1;
    }

    //  �㵽�߶���̾���
    inline glm::vec2 PointToSegment(const glm::vec2 & p, const glm::vec2 & a, const glm::vec2 & b)
    {
        auto ab = glm::vec3(b - a, 0);
        auto ap = glm::vec3(p - a, 0);
        auto bp = glm::vec3(p - b, 0);
        if      (glm::dot(ap, ab) <= 0) { return -ap; }
        else if (glm::dot(bp, ab) >= 0) { return -bp; }
        else
        {
            auto l = glm::length(ab);
            auto s = glm::dot(ap, ab) / l;
            return s / l * ab;
        }
    }

    //  �������ζ���˳��
    inline int CalePointsOrder(const std::vector<glm::vec2> & points)
    {
        ASSERT_LOG(points.size() >= 3, "");
        auto min = 0;
        auto ret = 0;
        for (auto i = 0; i != points.size(); ++i)
        {
            if (points.at(i).x<=points.at(min).x)
            {
                auto prev = (i + points.size() - 1) % points.size();
                auto next = (i + 1)                 % points.size();
                auto a = glm::vec3(points.at(i) - points.at(prev), 0);
                auto b = glm::vec3(points.at(next) - points.at(i), 0);
                auto z = glm::cross(a, b).z;
                if (z != 0) 
                {
                    ret = z < 0 ?  1
                        : z > 0 ? -1
                        : 0;
                    min = i; 
                }
            }
        }
        return ret;
    }

    //  ���ֶ����
    inline void BinaryPoints(
        const std::vector<glm::vec2> & points,
        const uint begA, const uint endA, const glm::vec2 & beg,
        const uint begB, const uint endB, const glm::vec2 & end,
        std::vector<glm::vec2> output[2])
    {
        output[0].push_back(beg);
        output[0].push_back(end);
        for (auto i = endB; i != endA; i = (i + 1) % points.size())
        {
            output[0].push_back(points.at(i));
        }

        output[1].push_back(beg);
        for (auto i = endA; i != endB; i = (i + 1) % points.size())
        {
            output[1].push_back(points.at(i));
        }
        output[1].push_back(end);
    }

    //  �и�պ�·��
    inline void StripClosePoints(const std::vector<glm::vec2> & points, std::vector<std::vector<glm::vec2>> & output)
    {
        if (points.size() < 3) { return; }

        //static auto CheckStripPoint = [](const std::vector<glm::vec2> & points, )
        //{
        //    
        //};

        for (auto i = 2; i != points.size(); ++i)
        {

        }
    }

    //  �и�պ�·��
    inline std::vector<std::vector<glm::vec2>> StripClosePoints(const std::vector<glm::vec2> & points)
    {
        std::vector<std::vector<glm::vec2>> result;
        StripClosePoints(points, result);
        return std::move(result);
    }

    //  �и�͹�����
    inline std::vector<std::vector<glm::vec2>> StripCullPoints(const std::vector<glm::vec2> & points)
    {
        return std::vector<std::vector<glm::vec2>>();
    }
}