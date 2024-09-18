class CountdownUI extends UIScriptedMenu
{
    private Widget m_CountdownUI;
    private TextWidget m_CountdownText;
    private TextWidget m_CountdownTimer;
    private bool m_IsCountdownVisible;
    private int m_Countdown;
    private int m_StartCountdown;

    void CountdownUI()
    {
       m_CountdownUI = GetGame().GetWorkspace().CreateWidgets("Battlegrounds/GUI/layouts/Countdown.layout");
       m_CountdownText = TextWidget.Cast(m_CountdownUI.FindAnyWidget("CountdownText"));
       m_CountdownTimer = TextWidget.Cast(m_CountdownUI.FindAnyWidget("CountdownTimer"));
       m_IsCountdownVisible = false;
       m_Countdown = 10;
    }

    void StartCountdown(int countdownTime = 10)
    {
        m_Countdown = countdownTime;
        m_StartCountdown = countdownTime;
        m_CountdownUI.Show(true);
        m_CountdownText.SetText("LEAVING SAFE ZONE:");
        m_CountdownTimer.SetText(m_Countdown.ToString());
        m_IsCountdownVisible = true;
        GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(UpdateCountdown, 1000, true);
    }

    void StopCountdown()
    {
        GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove(UpdateCountdown);

        m_CountdownUI.Show(false);
        m_CountdownText.SetText("");
        m_IsCountdownVisible = false;
    }

    void ForceStopCountdown()
    {
        StopCountdown();
        m_Countdown = m_StartCountdown;
    }

    void UpdateCountdown()
    {
        m_Countdown--;
        m_CountdownTimer.SetText(m_Countdown.ToString());
        if (m_Countdown <= 0)
        { StopCountdown(); }
    }

    bool IsCountdownVisible()
    { return m_IsCountdownVisible; }
}