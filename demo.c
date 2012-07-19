#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>

#include "calculate.c"

GtkWidget* text_rem;
GtkWidget* text_step;
GtkWidget* entry_out;
GtkWidget* box_output_e;

void set_output(int r){
	char buf[r*21];
	int i, j;
	strcpy(buf, "");
	for(i = 0; i < r; i++){
		char s[20];
		sprintf(s, "%f,", m[i][r]);
		strcat(buf, s);
	}
	gtk_entry_set_text(GTK_ENTRY(entry_out), buf);
}
void set_steps(){
	GtkTextBuffer *s = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_step));
	
	char *buf = get_start();
	if(buf != NULL) {
		gtk_text_buffer_set_text(s, buf, -1);
	}
}
void set_steps_back(){
	GtkTextBuffer *s = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_step));

	char *buf = get_back();
	if(buf != NULL) {
		gtk_text_buffer_set_text(s, buf, -1);
	}
}
void get(){
	empty_queues();
	GtkTextBuffer *b = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_rem));
	GtkTextBuffer *s = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_step));
	GtkTextIter start_i, end;
	gtk_text_buffer_get_start_iter (b, &start_i);
  	gtk_text_buffer_get_end_iter (b, &end);
	gchar *c = gtk_text_buffer_get_text(b, &start_i, &end, FALSE);
	int i;
	for(i = strlen(c) - 1; c[i] == '\n' || c[i] == '\0' ; i++) c[i] = '\0';

	int n = arg_count(c, '\n');
	alloc_m(n);

	char **line;
	line = (char **)malloc(sizeof(char *)*(n + 1));
	splitter(c, line, "\n");
	
	for(i = 0; i < n; i++){
		char **num;
		num = (char **)malloc(sizeof(char *)*(n + 2));
		
		int nm = splitter(line[i], num, ",");

		if(nm < n + 1){
			gtk_entry_set_text(GTK_ENTRY(entry_out), "INPUT ERROR : metrix error");
			gtk_text_buffer_set_text(s, "", -1);	
			return;
		}

		int j;
		for(j = 0; j < n + 1; j++)
			if(validate(num[j]))
				m[i][j] = atof(num[j]);
			else{
				free(num);
				free(line);
				free(c);
				gtk_entry_set_text(GTK_ENTRY(entry_out), 
						"INPUT ERROR : invalide characters");
				gtk_text_buffer_set_text(s, "", -1);	
				return;
			}
		free(num);
	}
	free(line);
	int flag = solve(n + 1, n);
	if(!flag)
		gtk_entry_set_text(GTK_ENTRY(entry_out), "INPUT ERROR!");
	else 
		set_output(n);

	if(start != NULL) {
		curnt = start;
		gtk_text_buffer_set_text(s, curnt -> s, -1);
	}else 
		gtk_text_buffer_set_text(s, "", -1);	
}
void clear(){
	GtkTextBuffer *b = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_rem));
	gtk_text_buffer_set_text(b, "", -1);

	GtkTextBuffer *c = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_step));
	gtk_text_buffer_set_text(c, "", -1);

	gtk_entry_set_text(GTK_ENTRY(entry_out), "");
}
int main(int argc, char **argv){

	GtkWidget *window_main;

	GtkWidget *box_main;
	GtkWidget *box_base;
	GtkWidget* box_input;
	GtkWidget* box_input_b;
	GtkWidget* box_input_t;
	GtkWidget* box_output;
	GtkWidget* box_output_b;
	GtkWidget* box_output_t;

	GtkWidget* button_clear;
	GtkWidget* button_solve;
	GtkWidget* button_for;
	GtkWidget* button_bak;

	GtkWidget* scr;
	GtkWidget* scr2;

	GtkWidget* image_header;

	GtkWidget* label_out;
	GtkWidget* label_in;
	GtkWidget* label_out_2;

	gtk_init(&argc, &argv);
	window_main = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title (GTK_WINDOW (window_main), "---Gauss-Jordan Elimination---");
	gtk_window_set_default_size(GTK_WINDOW(window_main), 800, 400);

	box_main     = gtk_vbox_new (FALSE, 0);
	box_base     = gtk_hbox_new (FALSE, 0);
	box_input    = gtk_vbox_new (FALSE, 0);
	box_input_b  = gtk_hbutton_box_new ();
	box_input_t  = gtk_hbox_new (FALSE, 0);

	box_output   = gtk_vbox_new (FALSE, 0);
	box_output_b = gtk_hbutton_box_new ();
	box_output_t = gtk_hbox_new (FALSE, 0);
	box_output_e = gtk_vbox_new (FALSE, 0);

	button_clear = gtk_button_new_with_label ("clear");
	button_solve = gtk_button_new_with_label ("solve");
	button_for   = gtk_button_new_with_label ("forword");
	button_bak   = gtk_button_new_with_label ("backword");
	
	entry_out    = gtk_entry_new();

	image_header = gtk_image_new();
	label_in     = gtk_label_new("input");
	label_out    = gtk_label_new("output");
	label_out_2  = gtk_label_new("steps");


	gtk_button_box_set_layout(GTK_BUTTON_BOX(box_input_b), GTK_BUTTONBOX_CENTER);
	gtk_box_set_spacing(GTK_BOX(box_input_b), 0);
	gtk_button_box_set_layout(GTK_BUTTON_BOX(box_output_b), GTK_BUTTONBOX_CENTER);
	gtk_box_set_spacing(GTK_BOX(box_output_b), 0);

	gtk_image_set_from_file (GTK_IMAGE (image_header), "header.jpg");

	gtk_box_pack_start (GTK_BOX (box_main), image_header, FALSE, FALSE, 1);
	gtk_widget_show (image_header);

	gtk_box_pack_start (GTK_BOX (box_input), label_in, FALSE, FALSE, 1);
	gtk_widget_show (label_in);

	gtk_signal_connect (GTK_OBJECT (button_clear), "clicked",
					GTK_SIGNAL_FUNC (clear), NULL);
	gtk_box_pack_start (GTK_BOX (box_input_b), button_clear, FALSE, FALSE, 1);
	gtk_widget_show (button_clear);

	gtk_signal_connect (GTK_OBJECT (button_solve), "clicked",
					GTK_SIGNAL_FUNC (get), NULL);
	gtk_box_pack_start (GTK_BOX (box_input_b), button_solve, FALSE, FALSE, 1);
	gtk_widget_show (button_solve);
	gtk_box_pack_start(GTK_BOX (box_input), box_input_b, FALSE, FALSE, 1);
	gtk_widget_show (box_input_b);

	text_rem = gtk_text_view_new();
	scr = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scr), text_rem);
	gtk_box_pack_start(GTK_BOX (box_input_t), scr, TRUE, TRUE, 1);
	gtk_box_pack_start(GTK_BOX (box_input), box_input_t, TRUE, TRUE, 1);
	gtk_box_pack_start(GTK_BOX (box_base), box_input, TRUE, TRUE, 1);

	gtk_widget_show(scr);
	gtk_widget_show(text_rem);
	gtk_widget_show(box_input);
	gtk_widget_show(box_input_t);

	
	gtk_box_pack_start (GTK_BOX (box_output), label_out_2, FALSE, FALSE, 1);
	gtk_widget_show (label_out_2);

	gtk_signal_connect (GTK_OBJECT (button_bak), "clicked",
					GTK_SIGNAL_FUNC (set_steps_back), NULL);
	gtk_box_pack_start (GTK_BOX (box_output_b), button_bak, TRUE, TRUE, 1);
	gtk_widget_show (button_bak);

	gtk_signal_connect (GTK_OBJECT (button_for), "clicked",
					GTK_SIGNAL_FUNC (set_steps), NULL);
	gtk_box_pack_start (GTK_BOX (box_output_b), button_for, TRUE, TRUE, 1);
	gtk_widget_show (button_for);
	gtk_box_pack_start(GTK_BOX (box_output), box_output_b, FALSE, FALSE, 1);
	gtk_widget_show (box_output_b);

	text_step = gtk_text_view_new();
	scr2 = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scr2), text_step);
	gtk_box_pack_start(GTK_BOX (box_output_t), scr2, TRUE, TRUE, 1);
	
	gtk_box_pack_start(GTK_BOX (box_output), box_output_t, TRUE, TRUE, 1);
	gtk_box_pack_start(GTK_BOX (box_base), box_output, TRUE, TRUE, 1);
	gtk_box_pack_start(GTK_BOX (box_main), box_base, TRUE, TRUE, 1);

	gtk_widget_show(scr2);
	gtk_widget_show(text_step);
	gtk_widget_show(box_output);
	gtk_widget_show(box_output_t);
	gtk_widget_show(box_base);

	gtk_box_pack_start(GTK_BOX (box_output_e), label_out, FALSE, FALSE, 1);
	gtk_widget_show(label_out);

	gtk_box_pack_start(GTK_BOX (box_output_e), entry_out, FALSE, FALSE, 1);
	gtk_widget_show(entry_out);
	gtk_box_pack_start(GTK_BOX (box_main), box_output_e, FALSE, FALSE, 3);
	gtk_widget_show(box_output_e);

	gtk_container_add(GTK_CONTAINER (window_main), box_main);
	gtk_widget_show(box_main);
	gtk_widget_show(window_main);

	gtk_main();
	return 0;
}
