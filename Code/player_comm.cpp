#include "player_comm.h"

GtkWidget *fromView  = NULL;  // text from the chat server

player_comm::player_comm(asio::io_context& io_context, const tcp::resolver::results_type& endpoints)
   : io_context_(io_context), socket_(io_context) 
{
	do_connect(endpoints);
}

void player_comm::write(const chat_message& msg)
{
  asio::post(io_context_,
      [this, msg]()
      {
        bool write_in_progress = !write_msgs_.empty();
        write_msgs_.push_back(msg);
        if (!write_in_progress)
        {
          do_write();
        }
   	 });
}

  void player_comm::close()
  {
    asio::post(io_context_, [this]() { socket_.close(); });
  }


  void player_comm::do_connect(const tcp::resolver::results_type& endpoints)
  {
    asio::async_connect(socket_, endpoints,
        [this](std::error_code ec, tcp::endpoint)
        {
          if (!ec)
          {
            do_read_header();
          }
        });
  }

  void player_comm::do_read_header()
  {
    asio::async_read(socket_,
        asio::buffer(read_msg_.data(), chat_message::header_length),
        [this](std::error_code ec, std::size_t /*length*/)
        {
          if (!ec && read_msg_.decode_header())
          {
            do_read_body();
          }
          else
          {
            socket_.close();
          }
        });
  }

  void player_comm::do_read_body()
  {
    asio::async_read(socket_,
        asio::buffer(read_msg_.body(), read_msg_.body_length()),
        [this](std::error_code ec, std::size_t /*length*/)
        {
          if (!ec)
          {
            char outline[read_msg_.body_length() + 2];
                                       // '\n' + '\0' is 2 more chars
            outline[0] = '\n';
            outline[read_msg_.body_length() + 1] = '\0';
            std::memcpy ( &outline[1], read_msg_.body(), read_msg_.body_length() );
            _message = nlohmann::json::parse(read_msg_.body());
            std::cout.write(read_msg_.body(), read_msg_.body_length());
            std::cout << "\n";
            do_read_header();
          }
          else
          {
            socket_.close();
          }
        });
  }

  void player_comm::do_write()
  {
    asio::async_write(socket_,
        asio::buffer(write_msgs_.front().data(),
          write_msgs_.front().length()),
        [this](std::error_code ec, std::size_t /*length*/)
        {
          if (!ec)
          {
            write_msgs_.pop_front();
            if (!write_msgs_.empty())
            {
              do_write();
            }
          }
          else
          {
            socket_.close();
          }
        });
  }


/* global symbols
chat_client *c;

static void destroy_event(GtkWidget *widget, gpointer data)
{
   gtk_main_quit();
}

static gboolean delete_event(GtkWidget *widget, GdkEvent *event, gpointer data)
{
   return FALSE; // must return false to trigger destroy event for window
}

static void clearCallback(GtkWidget *widget, GdkEventButton *event, gpointer callback_data)
{
   std::cerr << "clear button pressed: " << event->button << std::endl;
}

static void sendCallback(GtkWidget *widget, GdkEventButton *event, gpointer callback_data)
{
   std::cerr << "send button pressed: " << event->button << std::endl;

   GtkWidget *callbackWidget = (GtkWidget*) callback_data;
   assert ( callbackWidget );

   GtkTextBuffer *toBuffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (callbackWidget));

   GtkTextIter start;
   GtkTextIter end;

   gtk_text_buffer_get_start_iter ( toBuffer, &start );
   gtk_text_buffer_get_end_iter ( toBuffer, &end );

   gchar *text = gtk_text_buffer_get_text ( toBuffer, &start, &end, FALSE );

   if (text)
   {
      chat_message msg;
      msg.body_length ( strlen(text) );
      std::memcpy(msg.body(), text, msg.body_length());
      msg.encode_header();
      assert ( c );  // this is a global class
      c->write(msg);
   }
   gtk_text_buffer_set_text ( toBuffer, "", -1 );
}

int main(int argc, char *argv[])
{

   gtk_init( &argc, &argv);

   GtkWidget *window = NULL;

   GtkWidget *clearButton,*sendButton = NULL;
   GtkWidget *toView  = NULL;  // text to the chat server

   GtkTextBuffer *toBuffer = NULL;

   toView = gtk_text_view_new ();
   fromView = gtk_label_new (NULL);


   toBuffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (toView));


   gtk_text_buffer_set_text (toBuffer, "First message to be sent", -1);
   gtk_label_set_text (GTK_LABEL(fromView), "waiting for server");


   window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
   clearButton = gtk_button_new_with_label("Clear");
   sendButton = gtk_button_new_with_label("Send");


   GtkWidget *  vbox = gtk_vbox_new (false, 10);
   gtk_container_add(GTK_CONTAINER(window), vbox);


   gtk_box_pack_start(GTK_BOX(vbox),clearButton, TRUE,TRUE,0);
   gtk_box_pack_start(GTK_BOX(vbox),sendButton, TRUE,TRUE,0);
   gtk_box_pack_start(GTK_BOX(vbox),toView, TRUE,TRUE,0);
   gtk_box_pack_start(GTK_BOX(vbox),fromView, TRUE,TRUE,0);


   g_signal_connect(G_OBJECT(clearButton), "button_press_event", G_CALLBACK(clearCallback), fromView);
   g_signal_connect(G_OBJECT(sendButton), "button_press_event", G_CALLBACK(sendCallback), toView);


   gtk_widget_show_all(window);
   g_signal_connect(G_OBJECT(window), "delete_event", G_CALLBACK(delete_event), NULL);
   g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(destroy_event), NULL);



   if (argc != 3)
   {
      std::cerr << "Usage: chat_client <host> <port>\n";
      return 1;
   }



   return 0;
}
*/


