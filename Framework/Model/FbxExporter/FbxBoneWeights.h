#pragma once
#include "stdafx.h"

namespace Fbx
{
	struct FbxBlendWeight
	{
		D3DXVECTOR4 Indices = { 0,0,0,0 };
		D3DXVECTOR4 Weights = { 0,0,0,0 };

		void Set(int index, int boneIndex, float weight)
		{
			float i = (float)boneIndex;
			float w = weight;

			switch (index)
			{
				case 0: Indices.x = i; Weights.x = w; break;
				case 1: Indices.y = i; Weights.y = w; break;
				case 2: Indices.z = i; Weights.z = w; break;
				case 3: Indices.w = i; Weights.w = w; break;
			}
		}
	};

	struct FbxBoneWeights
	{
	private:
		using Pair = pair<int, float>;
		//todo : ����, ���Ͱ� ���� �ʿ��Ѱ�?
		//multimap<float, int> _boneWeights;
		vector<Pair> _boneWeights;
	public:
		//�� ����ġ ������ �����̳ʿ� ����
		//@param : �� ��ȣ
		//@param : �� ����ġ
		void AddBoneWeight(int boneIndex, float boneWeight)
		{
			if (boneWeight < 0.0f)
				return;

			bool isAdded = false;

			//�������� ����
			for (auto it = _boneWeights.begin();it!=_boneWeights.end();it++)
			{
				if (boneWeight > it->second)
				{
					_boneWeights.insert(it, Pair(boneIndex, boneWeight));
					isAdded = true;
					break;
				}
			}

			if (isAdded == false)
				_boneWeights.push_back(Pair(boneIndex, boneWeight));
		}

		void AddBoneWeights(const FbxBoneWeights& boneWeights)
		{
			for (auto&& boneWeight : boneWeights._boneWeights)
				AddBoneWeight(boneWeight.first, boneWeight.second);
		}

		void GetBlendWeights(OUT FbxBlendWeight& blendWeights) const
		{
			int i = 0;
			for (auto&& boneWeight : _boneWeights)
			{
				blendWeights.Set(i++, boneWeight.first, boneWeight.second);

				if (i > 4)
					return;
			}
		}

		//0~1 ���̷� ����ȭ
		void Normalize()
		{
			float totalWeight = 0.0f;

			int i = 0;
			auto it = _boneWeights.begin();
			while (it != _boneWeights.end())
			{
				if (i++ < 4)
					totalWeight += (it++)->second;
				else
					it = _boneWeights.erase(it);

				float scale = 1.0f / totalWeight;
				it = _boneWeights.begin();
				while (it != _boneWeights.end())
					(it++)->second *= scale;
			}
		}
	};
}