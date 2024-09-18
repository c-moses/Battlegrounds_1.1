modded class ZombieBase extends DayZInfected
{
    private ref BattlegroundsRewardManager m_RewardManager;

    void ZombieBase() 
    { m_RewardManager = BattlegroundsRewardManager.GetInstance(); }

	override void EEKilled(Object killer)
	{
		super.EEKilled(killer);

		PlayerBase playerKiller = PlayerBase.Cast(killer);
		if (!playerKiller)
		{
			if (killer.IsInherited(Weapon_Base))
			{
				playerKiller = PlayerBase.Cast(EntityAI.Cast(killer).GetHierarchyParent());
			}
			else if (killer.IsInherited(ItemBase))
			{
				playerKiller = PlayerBase.Cast(EntityAI.Cast(killer).GetHierarchyParent());
			}
		}

		if (playerKiller)
		{
			int currencyAmount = 50;
			CurrencyHelper helper = BattlegroundsRewardManager.GetInstance().GetCurrencyHelper();
			helper.AddCurrencyStackToPlayer(playerKiller, currencyAmount);
			m_RewardManager.UpdatePlayerStat(playerKiller, "cashEarned", currencyAmount);
			m_RewardManager.UpdatePlayerStat(playerKiller, "zombieKills", 1);
		}
	}
    
    bool IsTargetInSafeZone(EntityAI target)
    {
        PlayerBase player = PlayerBase.Cast(target);
        
        if (player)
        { return player.IsInSafezone() || (player.IsExitCountdownActive()); }

        return false;
    }

    override bool ChaseAttackLogic(int pCurrentCommandID, DayZInfectedInputController pInputController, float pDt)
    {
        m_ActualTarget = pInputController.GetTargetEntity();

        if (IsTargetInSafeZone(m_ActualTarget))
            return false;

        return super.ChaseAttackLogic(pCurrentCommandID,pInputController,pDt);
    }

    override bool FightAttackLogic(int pCurrentCommandID, DayZInfectedInputController pInputController, float pDt)
	{
		m_ActualTarget = pInputController.GetTargetEntity();

        if (IsTargetInSafeZone(m_ActualTarget))
            return false;

		return super.FightAttackLogic(pCurrentCommandID,pInputController,pDt);
	}
}