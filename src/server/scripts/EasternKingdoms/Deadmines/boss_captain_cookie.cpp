/*
* Copyright (C) 2008-2014 TrinityCore <http://www.trinitycore.org/>
* Copyright (C) 2011-2016 ArkCORE <http://www.arkania.net/>
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 2 of the License, or (at your
* option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
* more details.
*
* You should have received a copy of the GNU General Public License along
* with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "ScriptMgr.h"
#include "ScriptPCH.h"
#include "ScriptedCreature.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "SpellHistory.h"
#include "Group.h"
#include "deadmines.h"

enum Spells
{
    SPELL_WHO_IS_THAT                       = 89339,
    SPELL_SETIATED                          = 89267,
    SPELL_SETIATED_H                        = 92834,
    SPELL_NAUSEATED                         = 89732,
    SPELL_NAUSEATED_H                       = 92066,
    SPELL_ROTTEN_AURA                       = 89734,
    SPELL_ROTTEN_AURA_H                     = 95513,
    SPELL_ROTTEN_AURA_DMG                   = 89734,
    SPELL_ROTTEN_AURA_DMG_H                 = 92065,
    SPELL_CAULDRON                          = 89250,
    SPELL_CAULDRON_VISUAL                   = 89251,
    SPELL_CAULDRON_FIRE                     = 89252,


    SPELL_THROW_FOOD_TARGETING_CORN         = 89268,
    SPELL_THROW_FOOD_TARGETING_ROTTEN_CORN  = 89740,
    SPELL_THROW_FOOD_TARGETING_MELON        = 90561,
    SPELL_THROW_FOOD_TARGETING_ROTTEN_MELON = 90582,
    SPELL_THROW_FOOD_TARGETING_STEAK        = 90562,
    SPELL_THROW_FOOD_TARGETING_ROTTEN_STEAK = 90583,
    SPELL_THROW_FOOD_TARGETING_MYSTERY_MEAT = 90563,
    SPELL_THROW_FOOD_TARGETING_ROTTEN_MM    = 90584,
    SPELL_THROW_FOOD_TARGETING_LOAF         = 90564,
    SPELL_THROW_FOOD_TARGETING_ROTTEN_LOAF  = 90585,
    SPELL_THROW_FOOD_TARGETING_BUN          = 90565,
    SPELL_THROW_FOOD_TARGETING_ROTTEN_BUN   = 90586,


    SPELL_THROW_FOOD        = 89263,
    SPELL_THROW_FOOD_FORCE  = 89269,
    SPELL_THROW_FOOD_BACK   = 89264,
    SPELL_THROW_FOOD_01     = 90557,
    SPELL_THROW_FOOD_02     = 90560,
    SPELL_THROW_FOOD_03     = 90603,
    SPELL_THROW_FOOD_04     = 89739,
    SPELL_THROW_FOOD_05     = 90605,
    SPELL_THROW_FOOD_06     = 90556,
    SPELL_THROW_FOOD_07     = 90680,
    SPELL_THROW_FOOD_08     = 90559,
    SPELL_THROW_FOOD_09     = 90602,
    SPELL_THROW_FOOD_10     = 89263,
    SPELL_THROW_FOOD_11     = 90604,
    SPELL_THROW_FOOD_12     = 90555,
    SPELL_THROW_FOOD_13     = 90606,
};

enum Adds
{
    NPC_BABY_MURLOC         = 48672,

    NPC_CAULDRON            = 47754,

    NPC_BUN                 = 48301,
    NPC_MISTERY_MEAT        = 48297,
    NPC_BREAD_LOAF          = 48300,
    NPC_STEAK               = 48296,
    NPC_CORN                = 48006,
    NPC_MELON               = 48294,

    NPC_ROTTEN_SNEAK        = 48295,
    NPC_ROTTEN_CORN         = 48276,
    NPC_ROTTEN_LOAF         = 48299,
    NPC_ROTTEN_MELON        = 48293,
    NPC_ROTTEN_MISTERY_MEAT = 48298,
    NPC_ROTTEN_BUN          = 48302,
};

enum Events
{
    EVENT_THROW_FOOD    = 1,
    EVENT_CAULDRON_1    = 2,
    EVENT_CAULDRON_2    = 3,
    EVENT_MOVE          = 4,
};

const uint32 ThrowFoodSpells[12] = 
{
    SPELL_THROW_FOOD_TARGETING_CORN,
    SPELL_THROW_FOOD_TARGETING_ROTTEN_CORN,
    SPELL_THROW_FOOD_TARGETING_MELON,
    SPELL_THROW_FOOD_TARGETING_ROTTEN_MELON,
    SPELL_THROW_FOOD_TARGETING_STEAK,
    SPELL_THROW_FOOD_TARGETING_ROTTEN_STEAK,
    SPELL_THROW_FOOD_TARGETING_MYSTERY_MEAT,
    SPELL_THROW_FOOD_TARGETING_ROTTEN_MM,
    SPELL_THROW_FOOD_TARGETING_LOAF,
    SPELL_THROW_FOOD_TARGETING_ROTTEN_LOAF,
    SPELL_THROW_FOOD_TARGETING_BUN,
    SPELL_THROW_FOOD_TARGETING_ROTTEN_BUN
};

#define POINT_MOVE  1

const Position cauldronPos = {-64.07f, -820.27f, 41.17f, 0.0f};
const Position movePos = {-71.292213f, -819.792297f, 40.51f, 0.04f};

class boss_captain_cookie : public CreatureScript
{
public:
    boss_captain_cookie() : CreatureScript("boss_captain_cookie") { }

    struct boss_captain_cookieAI : public BossAI
    {
        boss_captain_cookieAI(Creature* creature) : BossAI(creature, BOSS_CAPTAIN_COOKIE) 
        { 
            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_GRIP, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_STUN, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FEAR, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_ROOT, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FREEZE, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_POLYMORPH, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_HORROR, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SAPPED, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_CHARM, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISORIENTED, true);
            me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_CONFUSE, true);
            me->setActive(true);
			m_instance = creature->GetInstanceScript(); 
        }

        InstanceScript* m_instance;
        EventMap m_events;
        uint32 m_phase;

        void Reset()
        {
            _Reset();
            me->SetReactState(REACT_AGGRESSIVE);
            DoCast(SPELL_WHO_IS_THAT);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
			m_instance->SetData(BOSS_CAPTAIN_COOKIE, NOT_STARTED);
            m_events.Reset();
            m_phase = 0;
        }
		
		void MoveInLineOfSight(Unit* who)
            {
                if (m_instance->GetBossState(BOSS_ADMIRAL_RIPSNARL) != DONE)
                    return;

                if (me->GetDistance(who) > 5.0f)
                    return;

                BossAI::MoveInLineOfSight(who);
            }

        void EnterCombat(Unit* /*who*/)
        {
            me->RemoveAura(SPELL_WHO_IS_THAT);
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            me->AttackStop();
            me->SetReactState(REACT_PASSIVE);

            events.ScheduleEvent(EVENT_MOVE, 1000);

            DoZoneInCombat();
			m_instance->SetData(BOSS_CAPTAIN_COOKIE, IN_PROGRESS);
        }
		
		void MovementInform(uint32 type, uint32 data)
            {
                if (type == POINT_MOTION_TYPE)
                    if (data == POINT_MOVE)
                        events.ScheduleEvent(EVENT_CAULDRON_1, 2000);
            }

        void JustDied(Unit* /*Killer*/)
        {
            _JustDied();

            if (IsHeroic())
                    me->SummonCreature(NPC_NOTE, notePos);
			m_instance->SetData(BOSS_CAPTAIN_COOKIE, DONE);
        }

        void UpdateAI(uint32 diff)
        {
            if (!UpdateVictim())
                return;
			events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_MOVE:
                            me->GetMotionMaster()->MovePoint(POINT_MOVE, movePos);
                            break;
                        case EVENT_CAULDRON_1:
                            me->CastSpell(centershipPos.GetPositionX(), centershipPos.GetPositionY(), centershipPos.GetPositionZ(), SPELL_CAULDRON, true);
                            events.ScheduleEvent(EVENT_CAULDRON_2, 2000);
                            break;
                        case EVENT_CAULDRON_2:
                            if (Creature* pCauldron = me->FindNearestCreature(NPC_CAULDRON, 20.0f))
                                //me->EnterVehicle(pCauldron);
                                me->GetMotionMaster()->MoveJump(pCauldron->GetPositionX(), pCauldron->GetPositionY(), pCauldron->GetPositionZ(), 5, 10);
                            events.ScheduleEvent(EVENT_THROW_FOOD, 3000);
                            break;
                        case EVENT_THROW_FOOD:
                            DoCastAOE(ThrowFoodSpells[urand(0, 11)]);
                            events.ScheduleEvent(EVENT_THROW_FOOD, 4000);
                            break;
                    }
                }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return GetDeadminesAI<boss_captain_cookieAI>(creature);
    }
};

class npc_captain_cookie_cauldron : public CreatureScript
{
    public:
        npc_captain_cookie_cauldron() : CreatureScript("npc_captain_cookie_cauldron") { }

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_captain_cookie_cauldronAI (pCreature);
        }

        struct npc_captain_cookie_cauldronAI : public ScriptedAI
        {
            npc_captain_cookie_cauldronAI(Creature* pCreature) : ScriptedAI(pCreature)
            {
                me->SetReactState(REACT_PASSIVE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            }

            void Reset() 
            {
                DoCast(me, SPELL_CAULDRON_VISUAL, true);
                DoCast(me, SPELL_CAULDRON_FIRE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_STUNNED);
            }
        };
};

class npc_captain_cookie_good_food : public CreatureScript
{
    public:
        npc_captain_cookie_good_food() : CreatureScript("npc_captain_cookie_good_food") { }
     
        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_captain_cookie_good_foodAI (pCreature);
        }

        bool OnGossipHello(Player* pPlayer, Creature* pCreature)
        {
            InstanceScript* pInstance = pCreature->GetInstanceScript();
            if (!pInstance)
                return true;
            if (pInstance->GetBossState(DATA_CAPTAIN_COOKIE) != IN_PROGRESS)
                return true;

            pPlayer->CastSpell(pPlayer, (pPlayer->GetMap()->IsHeroic() ? SPELL_SETIATED_H : SPELL_SETIATED), true);

            pCreature->DespawnOrUnsummon();
            return true;
        }

        struct npc_captain_cookie_good_foodAI : public ScriptedAI
        {
            npc_captain_cookie_good_foodAI(Creature* pCreature) : ScriptedAI(pCreature) 
            {
                pInstance = pCreature->GetInstanceScript();
            }
    
            void JustDied(Unit* killer)
            {
                me->DespawnOrUnsummon();
            }

            void UpdateAI(const uint32 diff)
            {
                if (!pInstance)
                    return;

                if (pInstance->GetBossState(DATA_CAPTAIN_COOKIE) != IN_PROGRESS)
                    me->DespawnOrUnsummon();
            }

        private:
            InstanceScript* pInstance;
     
        };
};

class npc_captain_cookie_bad_food : public CreatureScript
{
    public:
        npc_captain_cookie_bad_food() : CreatureScript("npc_captain_cookie_bad_food") { }
     
        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_captain_cookie_bad_foodAI (pCreature);
        }
     
        bool OnGossipHello(Player* pPlayer, Creature* pCreature)
        {
            InstanceScript* pInstance = pCreature->GetInstanceScript();
            if (!pInstance)
                return true;
            if (pInstance->GetBossState(DATA_CAPTAIN_COOKIE) != IN_PROGRESS)
                return true;

            pPlayer->CastSpell(pPlayer, (pPlayer->GetMap()->IsHeroic() ? SPELL_NAUSEATED_H : SPELL_NAUSEATED), true);
            
            pCreature->DespawnOrUnsummon();
            return true;
        }

        struct npc_captain_cookie_bad_foodAI : public ScriptedAI
        {
            npc_captain_cookie_bad_foodAI(Creature* pCreature) : ScriptedAI(pCreature) 
            {
                pInstance = pCreature->GetInstanceScript();
            }

            void JustDied(Unit* killer)
            {
                me->DespawnOrUnsummon();
            }

            void UpdateAI(const uint32 diff)
            {
                if (!pInstance)
                    return;

                if (pInstance->GetBossState(DATA_CAPTAIN_COOKIE) != IN_PROGRESS)
                    me->DespawnOrUnsummon();
            }
        private:
            InstanceScript* pInstance;
        };
};

class spell_captain_cookie_setiated : public SpellScriptLoader
{
    public:
        spell_captain_cookie_setiated() : SpellScriptLoader("spell_captain_cookie_setiated") { }


        class spell_captain_cookie_setiated_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_captain_cookie_setiated_SpellScript);


            void HandleScript(SpellEffIndex /*effIndex*/)
            {
                if (!GetCaster() || !GetHitUnit())
                    return;
                GetHitUnit()->RemoveAuraFromStack(SPELL_NAUSEATED);
                GetHitUnit()->RemoveAuraFromStack(SPELL_NAUSEATED_H);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_captain_cookie_setiated_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_captain_cookie_setiated_SpellScript();
        }
};

class spell_captain_cookie_nauseated : public SpellScriptLoader
{
    public:
        spell_captain_cookie_nauseated() : SpellScriptLoader("spell_captain_cookie_nauseated") { }


        class spell_captain_cookie_nauseated_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_captain_cookie_nauseated_SpellScript);


            void HandleScript(SpellEffIndex /*effIndex*/)
            {
                if (!GetCaster() || !GetHitUnit())
                    return;
                GetHitUnit()->RemoveAuraFromStack(SPELL_SETIATED);
                GetHitUnit()->RemoveAuraFromStack(SPELL_SETIATED_H);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_captain_cookie_nauseated_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_APPLY_AURA);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_captain_cookie_nauseated_SpellScript();
        }
};

class spell_captain_cookie_throw_food_targeting : public SpellScriptLoader
{
    public:
        spell_captain_cookie_throw_food_targeting() : SpellScriptLoader("spell_captain_cookie_throw_food_targeting") { }

        class spell_captain_cookie_throw_food_targeting_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_captain_cookie_throw_food_targeting_SpellScript);

            void HandleAfterHit()
            {
                if (!GetCaster() || !GetHitUnit())
                    return;

                uint32 spellId = 0;

                if (SpellInfo const* spellInfo = GetSpellInfo())
                    spellId = spellInfo->GetEffect(EFFECT_0)->BasePoints;
                if (!sSpellMgr->GetSpellInfo(spellId))
                    return;

                GetCaster()->CastSpell(GetHitUnit(), spellId);
            }

            void Register()
            {
                AfterHit += SpellHitFn(spell_captain_cookie_throw_food_targeting_SpellScript::HandleAfterHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_captain_cookie_throw_food_targeting_SpellScript();
        }
};

void AddSC_boss_captain_cookie()
{
    new boss_captain_cookie();
	new npc_captain_cookie_cauldron();
    new npc_captain_cookie_good_food();
    new npc_captain_cookie_bad_food();
    new spell_captain_cookie_setiated();
    new spell_captain_cookie_nauseated();
    new spell_captain_cookie_throw_food_targeting();
}
