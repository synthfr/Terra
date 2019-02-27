#include "Controls.hpp"

NS_HWM_BEGIN

void IRenderableWindowBase::Render(wxDC &dc)
{
    if(is_shown_ == false) { return; }
    doRender(dc);
}

ImageButton::ImageButton(wxWindow *parent,
                         bool is_3state,
                         wxImage normal,
                         wxImage hover,
                         wxImage pushed,
                         wxImage hover_pushed,
                         wxPoint pos,
                         wxSize size)
:   IRenderableWindow<>(parent, wxID_ANY, pos, size)
,   orig_normal_(normal)
,   orig_hover_(hover)
,   orig_pushed_(pushed)
,   orig_hover_pushed_(hover_pushed)
,   is_3state_(is_3state)
{
    SetMinSize(normal.GetSize());
    SetSize(normal.GetSize());
    Bind(wxEVT_ENTER_WINDOW, [this](auto &ev) {
        is_hover_ = true;
        Refresh();
    });
    
    Bind(wxEVT_LEAVE_WINDOW, [this](auto &ev) {
        is_hover_ = false;
        is_being_pressed_ = false;
        Refresh();
    });
    
    Bind(wxEVT_LEFT_DOWN, [this](auto &ev) {
        is_being_pressed_ = true;
        Refresh();
    });
    
    Bind(wxEVT_LEFT_DCLICK, [this](auto &ev) {
        is_being_pressed_ = true;
        Refresh();
    });
    
    Bind(wxEVT_LEFT_UP, [this](auto &ev) {
        if(!is_hover_) { return; }
        if(is_3state_) {
            is_pushed_ = !is_pushed_;
        } else {
            is_pushed_ = false;
        }
        is_being_pressed_ = false;
        
        wxEventType type;
        if(is_3state_) {
            type = wxEVT_TOGGLEBUTTON;
        } else {
            type = wxEVT_BUTTON;
        }
        wxCommandEvent new_ev(type, GetId());
        ProcessWindowEvent(new_ev);
        Refresh();
    });
    
    SetAutoLayout(true);
    Layout();
}

bool ImageButton::IsPushed() const { return is_pushed_; }
void ImageButton::SetPushed(bool status) { is_pushed_ = status; }

bool ImageButton::Layout()
{
    auto size = GetClientSize();
    normal_ = orig_normal_.Scale(size.x, size.y);
    hover_ = orig_hover_.Scale(size.x, size.y);
    pushed_ = orig_pushed_.Scale(size.x, size.y);
    hover_pushed_ = orig_hover_pushed_.Scale(size.x, size.y);
    
    return IRenderableWindow<>::Layout();
}

void ImageButton::doRender(wxDC &dc)
{
    if(IsShown() == false) { return; }
    
    wxImage img;
    if(is_being_pressed_) {
        img = pushed_;
    } else if(is_hover_) {
        if(is_pushed_) {
            img = hover_pushed_;
        } else {
            img = hover_;
        }
    } else if(is_pushed_) {
        img = pushed_;
    } else {
        img = normal_;
    }
    
    dc.DrawBitmap(img, 0, 0);
}

//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------

Label::Label(wxWindow *parent)
:   IRenderableWindow<>(parent)
{
    Bind(wxEVT_LEFT_DOWN, [this](auto &ev) { ev.ResumePropagation(1); ev.Skip(); });
    Bind(wxEVT_LEFT_UP, [this](auto &ev) { ev.ResumePropagation(1); ev.Skip(); });
    Bind(wxEVT_LEFT_DCLICK, [this](auto &ev) { ev.ResumePropagation(1); ev.Skip(); });
    Bind(wxEVT_RIGHT_DOWN, [this](auto &ev) { ev.ResumePropagation(1); ev.Skip(); });
    Bind(wxEVT_RIGHT_UP, [this](auto &ev) { ev.ResumePropagation(1); ev.Skip(); });
    Bind(wxEVT_RIGHT_DCLICK, [this](auto &ev) { ev.ResumePropagation(1); ev.Skip(); });
    Bind(wxEVT_MIDDLE_DOWN, [this](auto &ev) { ev.ResumePropagation(1); ev.Skip(); });
    Bind(wxEVT_MIDDLE_UP, [this](auto &ev) { ev.ResumePropagation(1); ev.Skip(); });
    Bind(wxEVT_MIDDLE_DCLICK, [this](auto &ev) { ev.ResumePropagation(1); ev.Skip(); });
    Bind(wxEVT_MOTION, [this](auto &ev) { ev.ResumePropagation(1); ev.Skip(); });
}

bool Label::AcceptsFocus() const { return false; }

void Label::doRender(wxDC &dc)
{
    dc.DrawLabel(text_, GetClientRect(), align_);
}

void Label::SetText(wxString new_text)
{
    text_ = new_text;
    Refresh();
}

wxString Label::GetText() const
{
    return text_;
}

void Label::SetAlignment(int align)
{
    align_ = align;
}

int Label::GetAlignment() const
{
    return align_;
}

NS_HWM_END