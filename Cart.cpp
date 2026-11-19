#include "Commands/Cart.h"

#include <utility>

#include "LibraryLogger.h"
#include "Members/Member.h"

Cart::Cart(const Member &owner)
    : owner_{owner} {
}

void Cart::Record(const std::string &tag, const std::string &detail, std::string &output) const {
    output = tag + " " + owner_.GetName() + " : " + detail;
    LibraryLogger::GetInstance().Log(output);
}

// Comparaison d'adresses : la commande de Marie n'a rien a faire dans le
// panier de Hugo. Refusee, elle est detruite par l'appelant (unique_ptr).
bool Cart::Accepts(const ICommand &command, std::string &output) const {
    if (&command.GetMember() == &owner_)
        return true;
    output = "[REFUS] panier de " + owner_.GetName() + " : " + command.Describe()
             + " pour " + command.GetMember().GetName();
    LibraryLogger::GetInstance().Log(output);
    return false;
}

// Un ordre vide (tout a ete refuse) n'a rien a defaire : il n'est pas garde.
void Cart::Keep(std::vector<std::unique_ptr<ICommand>> order) {
    if (!order.empty())
        orders_.push_back(std::move(order));
}

// La commande attend : rien ne change avant la validation.
bool Cart::Add(std::unique_ptr<ICommand> command, std::string &output) {
    if (!Accepts(*command, output))
        return false;
    Record("[PANIER]", command->Describe(), output);
    pending_.push_back(std::move(command));
    return true;
}

// Retire la commande la plus recente, sans l'executer.
bool Cart::Cancel(std::string &output) {
    if (pending_.empty()) {
        output = "panier de " + owner_.GetName() + " : aucune commande a retirer";
        return false;
    }
    const std::unique_ptr<ICommand> latest = std::move(pending_.back());
    pending_.pop_back();
    Record("[RETRAIT]", latest->Describe(), output);
    return true;
}

// Execute les commandes dans l'ordre d'ajout ; les reussies forment un ordre.
// Retourne le nombre de commandes executees.
int Cart::Checkout(std::string &output) {
    const int total = static_cast<int>(pending_.size());
    std::string summary;
    Record("[VALIDATION]", std::to_string(total) + " commande(s)", summary);

    std::vector<std::unique_ptr<ICommand>> order;
    std::string detail;
    for (std::unique_ptr<ICommand> &command: pending_) {
        if (command->Execute(detail))
            order.push_back(std::move(command));
    }
    pending_.clear();              // les refusees n'ont rien a defaire : detruites ici

    const int executed = static_cast<int>(order.size());
    output = summary + " -> " + std::to_string(executed) + " executee(s), "
             + std::to_string(total - executed) + " refusee(s)";
    Keep(std::move(order));
    return executed;
}

// "Emprunter maintenant" : un ordre d'une seule commande, executee tout de suite.
bool Cart::ExecuteNow(std::unique_ptr<ICommand> command, std::string &output) {
    if (!Accepts(*command, output))
        return false;
    if (!command->Execute(output))
        return false;
    std::vector<std::unique_ptr<ICommand>> order;
    order.push_back(std::move(command));
    Keep(std::move(order));
    return true;
}

// Defait le dernier ordre, de sa derniere operation a la premiere. Un echec
// n'arrete rien : les autres operations sont quand meme defaites.
bool Cart::Undo(std::string &output) {
    if (orders_.empty()) {
        output = "panier de " + owner_.GetName() + " : aucune commande a annuler";
        return false;
    }
    const std::vector<std::unique_ptr<ICommand>> order = std::move(orders_.back());
    orders_.pop_back();

    std::string summary;
    Record("[ANNULATION COMMANDE]", std::to_string(order.size()) + " operation(s)", summary);
    bool allUndone = true;
    std::string failure;
    std::string detail;
    for (auto command = order.rbegin(); command != order.rend(); ++command) {
        if (!(*command)->Undo(detail)) {
            allUndone = false;
            failure = detail;
        }
    }
    output = allUndone ? summary + " -> toutes annulees" : failure;
    return allUndone;
}                                  // l'ordre et ses commandes sont detruits ici
