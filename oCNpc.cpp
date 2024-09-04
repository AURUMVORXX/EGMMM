// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
	
	void oCNpc::setHolded(bool holded)
	{
		m_holded = holded;

		if (m_holded == true)
		{
			ogame->addHoldedNpc(this);
			dontWriteIntoArchive = TRUE;
		}
		else
		{
			ogame->removeHoldedNpc(this);
			dontWriteIntoArchive = FALSE;
		}
	}
	const bool oCNpc::isHolded()					{ return m_holded; }

	void oCNpc::setDontArchive(bool holded)		{ m_dontArchive = holded; }
	const bool oCNpc::isDontArchive()				{ return m_dontArchive; }

	// ----------------------------------------------------

	HOOK Ivk_oCNpc AS(0x0072D950, &oCNpc::oCNpc_IVK);
	oCNpc* oCNpc::oCNpc_IVK()
	{
		m_holded			= false;
		m_dontArchive		= false;

		return THISCALL(Ivk_oCNpc)();
	}

	HOOK Ivk_oCNpc_Disable AS(&oCNpc::Disable, &oCNpc::Disable_IVK);
	void oCNpc::Disable_IVK()
	{
		if (isHolded())
			return;

		THISCALL(Ivk_oCNpc_Disable)();
	}

	HOOK Ivk_oCNpc_Equip AS(&oCNpc::Equip, &oCNpc::Equip_IVK);
	void oCNpc::Equip_IVK(oCItem* item)
	{
		if (ogame->isInfiniteRings() && item)
		{
			int category = inventory2.GetCategory(item);

			if (category == INV_MAGIC && item->HasFlag(ITM_FLAG_RING))
			{
				if (!item->HasFlag(ITM_FLAG_ACTIVE))
				{
					AddItemEffects(item);
					item->SetFlag(ITM_FLAG_ACTIVE);
				}
				else
				{
					RemoveItemEffects(item);
					item->ClearFlag(ITM_FLAG_ACTIVE);
				}

				return;
			}
		}

		THISCALL(Ivk_oCNpc_Equip)(item);
	}

	HOOK Ivk_oCNpc_Archive AS(&oCNpc::Archive, &oCNpc::Archive_IVK);
	void oCNpc::Archive_IVK(zCArchiver& arc)
	{
		THISCALL(Ivk_oCNpc_Archive)(arc);

		if (isHolded())
		{
			// Sleeping
			arc.WriteInt("isSleeping", sleepingMode == zVOB_SLEEPING);

			// Body visuals
			arc.WriteString("body_visualName", body_visualName);
			arc.WriteInt("body_texVarNr", body_TexVarNr);
			arc.WriteInt("body_texColorNr", body_TexColorNr);

			// Head visuals
			arc.WriteString("head_visualName", head_visualName);
			arc.WriteInt("head_texVarnr", head_TexVarNr);

			// Teeth visuals
			arc.WriteInt("teeth_TexVarNr", teeth_TexVarNr);

			// Anims
			zCModel* model = GetModel();

			arc.WriteInt("numActiveAnims", model->numActiveAnis);
			
			for (int i = 0; i < model->numActiveAnis; i++)
			{
				arc.WriteString("aniName", model->aniChannels[i]->protoAni->aniName);
				arc.WriteFloat("aniFrame", model->aniChannels[i]->actFrame);
			}

			// Face anims
			zCModelNodeInst* head	= model->SearchNode("BIP01 HEAD");
			zCMorphMesh* headMesh	= zDYNAMIC_CAST<zCMorphMesh>(head->nodeVisual);

			if (headMesh)
			{
				arc.WriteInt("numActiveFaceAnims", headMesh->aniChannels.GetNum());

				for (int i = 0; i < headMesh->aniChannels.GetNum(); i++)
				{
					arc.WriteString("aniName", headMesh->aniChannels[i]->ani->aniName);
					arc.WriteFloat("aniFrame", headMesh->aniChannels[i]->actFrame);
				}
			}
			else
			{
				arc.WriteInt("numActiveFaceAnims", 0);
			}
		}
	}

	HOOK Ivk_oCNpc_Unarchive AS(&oCNpc::Unarchive, &oCNpc::Unarchive_IVK);
	void oCNpc::Unarchive_IVK(zCArchiver& arc)
	{
		THISCALL(Ivk_oCNpc_Unarchive)(arc);

		if (isHolded())
		{
			Enable(GetPositionWorld());

			// *** Sleeping ***
			zBOOL isSleeping	= arc.ReadInt("isSleeping");

			// *** Body visuals ***
			body_visualName		= arc.ReadString("body_visualName");
			body_TexVarNr		= arc.ReadInt("body_texVarNr");
			body_TexColorNr		= arc.ReadInt("body_texColorNr");

			// *** Head visuals ***
			head_visualName		= arc.ReadString("head_visualName");
			head_TexVarNr		= arc.ReadInt("head_texVarnr");

			// *** Teeth visuals ***
			teeth_TexVarNr		= arc.ReadInt("teeth_TexVarNr");

			if (GetModel())
				InitModel();

			// *** Anims *** 
			zCModel* model		= GetModel();
			int numActiveAnims	= arc.ReadInt("numActiveAnims");

			zSTRING aniName;
			float aniFrame;

			for (int i = 0; i < numActiveAnims; i++)
			{
				aniName = arc.ReadString("aniName");
				aniFrame = arc.ReadFloat("aniFrame");

				// Set blend speed for instant anim apply + needed frame
				zCModelAni* ani			= model->GetAniFromAniID(model->GetAniIDFromAniName(aniName));
				ani->blendInSpeed		= zMDL_ANI_BLEND_IN_ZERO;
				ani->blendOutSpeed		= zMDL_ANI_BLEND_OUT_ZERO;
				model->StartAni(ani, 0);

				for (int i = 0; i < model->numActiveAnis; i++)
				{
					if (model->aniChannels[i]->protoAni->aniName == aniName)
					{
						model->aniChannels[i]->actFrame = aniFrame;
						break;
					}
				}
			}

			// *** Face Anims ***

			int numActiveFaceAnims = arc.ReadInt("numActiveFaceAnims");

			zCModelNodeInst* head = model->SearchNode("BIP01 HEAD");
			zCMorphMesh* headMesh = zDYNAMIC_CAST<zCMorphMesh>(head->nodeVisual);
			zCMorphMesh::zTMorphAniEntry* faceAni;

			if (headMesh)
			{
				for (int i = 0; i < numActiveFaceAnims; i++)
				{
					aniName		= arc.ReadString("aniName");
					aniFrame	= arc.ReadFloat("aniFrame");

					headMesh->StartAni(aniName, 1.0F, -2);

					for (int j = 0; j < headMesh->aniChannels.GetNum(); j++)
					{
						faceAni = headMesh->aniChannels[i];
						if (headMesh->aniChannels[j]->ani->aniName == aniName)
						{
							faceAni->actFrame		= aniFrame;
							faceAni->actFrameInt	= int(aniFrame);
							faceAni->nextFrameInt	= int(aniFrame) + 1;
							faceAni->blendInSpeed	= zMDL_ANI_BLEND_IN_ZERO;
							faceAni->blendOutSpeed	= zMDL_ANI_BLEND_OUT_ZERO;
						}
					}
				}
			}

			// Need to refresh anims and position in case of instant sleeping
			int lastFrameTime		= ztimer->frameTime;
			float lastFrameTimeF	= ztimer->frameTimeFloat;
			ztimer->frameTime		= 1;
			ztimer->frameTimeFloat	= 0.0001f;

			DoFrameActivity();

			ztimer->frameTime		= lastFrameTime;
			ztimer->frameTimeFloat	= lastFrameTimeF;

			// Apply sleeping
			SetSleeping(isSleeping);
		}
	}
}