#pragma once
#include <string>
#include <vector>
#include <memory>
#include <ostream>
#include "Player.h"

class Echipa {
private:
    std::string nume;
    std::string oras;
    std::string conferinta;
    double salaryCap;
    std::vector<std::unique_ptr<Player>> roster;

    double calculeazaSalariiTotale() const;

public:
    Echipa(const std::string& nume, const std::string& oras,
           const std::string& conferinta, double salaryCap);

    Echipa(const Echipa& other);
    Echipa& operator=(Echipa other);
    ~Echipa() = default;

    const std::string& getNume() const;
    const std::string& getOras() const;
    const std::string& getConferinta() const;
    double getSalaryCap() const;
    const std::vector<std::unique_ptr<Player>>& getRoster() const;

    void adaugaJucator(const Player& player);
    double getScorImpact() const;
    bool esteSubSalaryCap() const;
    int getNrJucatoriMaxContract() const;
    const Player& getCelMaiBunJucator() const;

    // lista jucatorilor All-Star din echipa
    std::vector<const Player*> getJucatoriAllStar() const;
    // numarul de All-Stars din echipa
    int getNrAllStars() const;
    // returneaza true daca echipa are minim 2 All-Stars
    bool isContender() const;
    // numarul de jucatori cu contract expiring
    int getNrJucatoriExpiring() const;
    double getValoareRoster() const;

    friend std::ostream& operator<<(std::ostream& os, const Echipa& echipa);
    friend void swap(Echipa& a, Echipa& b) noexcept;
};