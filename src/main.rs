use cursive::Cursive;
use cursive::views::Dialog;

fn main() {
    let mut siv = cursive::default();

    siv.add_layer(Dialog::new()
        .title("Caravan")
        .padding_lrtb(2, 2, 1, 1)
    );

    siv.run();
}
