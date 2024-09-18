class BattlegroundsBlueLoadout extends BattlegroundsLoadoutCrate1
{
    override void EEItemAttached(EntityAI item, string slot_name)
	{
        super.EEItemAttached(item, slot_name);
        if (item.IsKindOf("GreenTeamFlagItem") || item.IsKindOf("RedTeamFlagItem") || item.IsKindOf("OrangeTeamFlagItem"))
        {
            ReplenishBaseSupplies();
            AddCaptureRewards();
            SendFlagClaimedRPC(item, "blueteamclaim");
        }
    }

    void SendFlagClaimedRPC(EntityAI item, string action)
    {
        string teamColor;
        if (item.IsKindOf("GreenTeamFlagItem")) 
        { teamColor = "green"; } 
        else if (item.IsKindOf("RedTeamFlagItem")) 
        { teamColor = "red"; } 
        else if (item.IsKindOf("OrangeTeamFlagItem")) 
        { teamColor = "orange"; }
        else { return; }

        if (teamColor != "") 
        {   
            // 'teamColor' specifies which team's flag was claimed
            // 'action' specifies which team claimed it (e.g., "blueteamclaim")
            ref Param2<string, string> flagClaimedData = new Param2<string, string>(teamColor, action);
            GetRPCManager().SendRPC("Battlegrounds", "FlagClaimed", flagClaimedData, true, null);
        }
    }

    override void ReplenishBaseSupplies()
    {
        ClearInventory();
        ClearAttachments();
        array<string> itemsCargo = {"Hatchet", "NailBox", "BlueTeamLock", "LargeTent"};
        array<int> quantitiesCargo = {3, 4, 2, 1};
        array<string> itemsAttachments = {"AliceBag_Black"};
        array<int> quantitiesAttachments = {3};
        if (itemsCargo.Count() == quantitiesCargo.Count())
        {
            for (int i = 0; i < itemsCargo.Count(); i++)
            {
                string itemCargo = itemsCargo[i];
                int quantityCargo = quantitiesCargo[i];
                for (int j = 0; j < quantityCargo; j++)
                { GetInventory().CreateInInventory(itemCargo); }
            }
        }

        if (itemsAttachments.Count() == quantitiesAttachments.Count())
        {
            for (int k = 0; k < itemsAttachments.Count(); k++)
            {
                string itemAttachment = itemsAttachments[k];
                int quantityAttachment = quantitiesAttachments[k];
                for (int l = 0; l < quantityAttachment; l++)
                {
                    EntityAI attachment;
                    if (itemAttachment == "AliceBag_Black")
                    {
                        attachment = GetInventory().CreateAttachment(itemAttachment);
                        if (attachment && attachment.GetInventory())
                        {
                            // maybe
                        }
                    }
                }
            }
        }
    }
};