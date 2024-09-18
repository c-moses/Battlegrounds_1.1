modded class NotificationUI
{
    override void UpdateOffset()
    {
        UIScriptedMenu menu = UIScriptedMenu.Cast(GetGame().GetUIManager().GetMenu());

        if (!menu)
        {
            //Print("NotificationUI: Menu is NULL in UpdateOffset");
            return;
        }

        Widget layoutRoot = menu.GetLayoutRoot();
        if (!layoutRoot)
        {
            //Print("NotificationUI: LayoutRoot is NULL in UpdateOffset");
            return;
        }

        Widget expNotification = menu.GetLayoutRoot().FindAnyWidget("notification_root");
        if (!expNotification)
        {
            //Print("NotificationUI: expNotification is NULL in UpdateOffset");
            return;
        }

        if (expNotification.IsVisible())
        {
            if (!m_OffsetEnabled)
            {
                m_Root.GetPos(m_BackupPosX, m_BackupPosY);

                float x, y, w, h;
                m_Root.GetScreenPos(x, y);
                expNotification.GetScreenSize(w, h);

                m_Root.SetScreenPos(x, h);
                m_OffsetEnabled = true;
            }
        }
        else if (m_OffsetEnabled)
        {
            m_Root.SetPos(m_BackupPosX, m_BackupPosY);
            m_OffsetEnabled = false;
        }
    }
}